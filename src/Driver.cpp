#include <comms_lora_ebyte_e32/Driver.hpp>
#include <base-logging/Logging.hpp>
#include <iostream>

using namespace comms_lora_ebyte_e32;
using std::to_string;

Driver::Driver()
    : iodrivers_base::Driver(512) {
}

int Driver::extractPacket(uint8_t const* buffer, size_t buffer_size) const {
    throw std::logic_error("this drivers should only be used in raw mode");
}

Version Driver::readVersion() {
    uint8_t cmd[3] = { 0xc3, 0xc3, 0xc3 };
    iodrivers_base::Driver::writePacket(cmd, 3);

    uint8_t reply[4];
    int i = iodrivers_base::Driver::readRaw(
        reply, 4,
        base::Time::fromSeconds(1),
        base::Time::fromSeconds(1),
        base::Time::fromSeconds(1)
    );
    if (i != 4) {
        throw iodrivers_base::TimeoutError(
            i == 0 ? iodrivers_base::TimeoutError::FIRST_BYTE
                   : iodrivers_base::TimeoutError::PACKET,
            "comms_lora_ebyte_e32::Driver::readVersion: did not receive reply"
        );
    }

    if (reply[0] != 0xc3) {
        throw std::runtime_error(
            "unexpected first byte in reply to readVersion, expected " +
            std::to_string(static_cast<int>(0xc3)) + " but got " +
            std::to_string(static_cast<int>(reply[0]))
        );
    }

    Version version;
    switch (reply[1]) {
        case 32:
            version.frequency = FREQ_433MHZ;
            break;
        case 38:
            version.frequency = FREQ_470MHZ;
            break;
        case 45:
            version.frequency = FREQ_868MHZ;
            break;
        case 44:
            version.frequency = FREQ_915MHZ;
            break;
        case 46:
            version.frequency = FREQ_170MHZ;
            break;
        default:
            LOG_ERROR_S << "comms_lora_ebyte_e32::Driver::readVersion(): "
                        << "invalid frequency code "
                        << std::dec << static_cast<int>(reply[1]) << std::endl;
            version.frequency = FREQ_INVALID;
            break;
    }

    version.version = reply[2];
    version.features = reply[3];
    return version;
}

Configuration Driver::decodeConfiguration(uint8_t const* data) {
    Configuration conf;
    uint16_t address_msb = data[0];
    uint16_t address_lsb = data[1];
    uint8_t uart_parity_raw = (data[2] >> 6) & 0b11;
    if (uart_parity_raw == 3) {
        uart_parity_raw = 0;
    }
    uint8_t uart_rate_raw = (data[2] >> 3) & 0b111;
    uint8_t air_rate_raw = (data[2] >> 0) & 0b111;
    if (air_rate_raw > Configuration::AIR_RATE_19200) {
        air_rate_raw = Configuration::AIR_RATE_19200;
    }
    uint8_t channel = data[3];
    uint8_t transmission_mode_raw = (data[4] >> 7) & 0b1;
    uint8_t io_drive_mode_raw = (data[4] >> 6) & 0b1;
    uint8_t wireless_wakeup_time_raw = (data[4] >> 3) & 0b111;
    uint8_t error_correction_enabled = (data[4] >> 2) & 0b1;
    uint8_t transmission_power_raw = (data[4] >> 0) & 0b11;

    conf.address = address_msb << 8 | address_lsb;
    conf.uart_parity = static_cast<Configuration::UARTParity>(uart_parity_raw);
    conf.uart_rate = static_cast<Configuration::UARTRate>(uart_rate_raw);
    conf.air_rate = static_cast<Configuration::AirRate>(air_rate_raw);
    conf.channel = channel;
    conf.transparent_transmission = transmission_mode_raw == 0;
    conf.io_drive_mode = io_drive_mode_raw ? Configuration::IO_PUSH_PULL :
                                             Configuration::IO_OPEN_DRAIN;
    conf.wireless_wake_up_time = wireless_wakeup_time_raw;
    conf.error_correction_enabled = error_correction_enabled;
    conf.transmission_power = static_cast<Configuration::TransmissionPower>(
        transmission_power_raw
    );

    return conf;
}

Configuration Driver::readConfiguration() {
    uint8_t cmd[3] = { 0xc1, 0xc1, 0xc1 };
    iodrivers_base::Driver::writePacket(cmd, 3);

    static const int REPLY_SIZE = 6;
    static const int EXPECTED_FIRST_BYTE = 0xc0;

    uint8_t reply[REPLY_SIZE];
    int i = iodrivers_base::Driver::readRaw(
        reply, REPLY_SIZE,
        base::Time::fromSeconds(1),
        base::Time::fromSeconds(1),
        base::Time::fromSeconds(1)
    );
    if (i != REPLY_SIZE) {
        throw iodrivers_base::TimeoutError(
            i == 0 ? iodrivers_base::TimeoutError::FIRST_BYTE
                   : iodrivers_base::TimeoutError::PACKET,
            "comms_lora_ebyte_e32::Driver::readConfiguration: did not receive reply"
        );
    }

    if (reply[0] != EXPECTED_FIRST_BYTE) {
        throw std::runtime_error(
            "unexpected first byte in data to readConfiguration, expected " +
            std::to_string(static_cast<int>(EXPECTED_FIRST_BYTE)) + " but got " +
            std::to_string(static_cast<int>(reply[0]))
        );
    }

    return decodeConfiguration(reply + 1);
}

void Driver::encodeConfiguration(uint8_t* buffer, Configuration const& conf) {
    buffer[0] = (conf.address >> 8) & 0xff;
    buffer[1] = (conf.address >> 0) & 0xff;
    buffer[2] = (conf.uart_parity << 6) |
                (conf.uart_rate << 3) |
                (conf.air_rate << 0);
    buffer[3] = conf.channel;
    buffer[4] = ((conf.transparent_transmission ? 0 : 1) << 7) |
                ((conf.io_drive_mode == Configuration::IO_PUSH_PULL) << 6) |
                (conf.wireless_wake_up_time << 3) |
                ((conf.error_correction_enabled ? 1 : 0) << 2) |
                (conf.transmission_power << 0);
}

void Driver::writeConfiguration(Configuration const& conf, bool save) {
    uint8_t buffer[6];
    buffer[0] = save ? 0xc0 : 0xc2;
    encodeConfiguration(buffer + 1, conf);
    iodrivers_base::Driver::writePacket(buffer, 6);
}
