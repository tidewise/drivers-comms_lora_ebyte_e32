#include <iostream>
#include <comms_lora_ebyte_e32/Driver.hpp>

using namespace std;
using namespace comms_lora_ebyte_e32;

void usage(std::ostream& os) {
    os << "comms_lora_ebyte_e32_ctl URI CMD ARGS\n"
       << "  version: displays the version info\n"
       << "  show: display current configuration\n"
       << "  set VAR VALUE: set a configuration variable (non permanent)\n"
       << "  save: save the current configuration\n"
       << "\n"
       << "Configuration parameters:\n"
       << "  uart-parity: 8N1, 8E1 or 8O1\n"
       << "  uart-rate: 1200, 2400, 4800, 9600, 19200, 38400, 57600 or 115200\n"
       << "  air-rate: 300, 1200, 2400, 4800, 9600, 19200\n"
       << "  channel: 1MHz increment above 410MHz, up to 31 (441MHz)\n"
       << "  transmission-mode: either fixed or transparent\n"
       << "  io-drive-mode: either OPEN_DRAIN or PUSH_PULL\n"
       << "  wireless-wake-up-time: increments of 250ms above 250ms, "
          "e.g. '2' means 500ms\n"
       << "  error-correction: either 'enabled' or 'disabled'\n"
       << "  transmission-power: one of 30dBm, 27dBm, 24dBm or 21dBm\n"
       << flush;
}

#define CASE_TO_STRING(s) \
    case Configuration::s: \
        return #s;

string to_string(Configuration::UARTParity parity) {
    switch (parity) {
        CASE_TO_STRING(PARITY_8N1);
        CASE_TO_STRING(PARITY_8O1);
        CASE_TO_STRING(PARITY_8E1);
    }
    throw std::invalid_argument("unexpected value for UART parity");
}

string to_string(Configuration::UARTRate rate) {
    switch (rate) {
        CASE_TO_STRING(RATE_1200);
        CASE_TO_STRING(RATE_2400);
        CASE_TO_STRING(RATE_4800);
        CASE_TO_STRING(RATE_9600);
        CASE_TO_STRING(RATE_19200);
        CASE_TO_STRING(RATE_38400);
        CASE_TO_STRING(RATE_57600);
        CASE_TO_STRING(RATE_115200);
    }
    throw std::invalid_argument("unexpected value for UART rate");
}

string to_string(Configuration::AirRate rate) {
    switch (rate) {
        CASE_TO_STRING(AIR_RATE_300);
        CASE_TO_STRING(AIR_RATE_1200);
        CASE_TO_STRING(AIR_RATE_2400);
        CASE_TO_STRING(AIR_RATE_4800);
        CASE_TO_STRING(AIR_RATE_9600);
        CASE_TO_STRING(AIR_RATE_19200);
    }
    throw std::invalid_argument("unexpected value for air rate");
}

string to_string(Configuration::IODriveMode mode) {
    switch (mode) {
        CASE_TO_STRING(IO_OPEN_DRAIN);
        CASE_TO_STRING(IO_PUSH_PULL);
    }
    throw std::invalid_argument("unexpected value for IO mode");
}

string to_string(Configuration::TransmissionPower power) {
    switch (power) {
        CASE_TO_STRING(POWER_30dBm);
        CASE_TO_STRING(POWER_27dBm);
        CASE_TO_STRING(POWER_24dBm);
        CASE_TO_STRING(POWER_21dBm);
    }
    throw std::invalid_argument("unexpected value for transmission power");
}

void conf_show(Configuration const& conf) {
    cout << "Address: " << conf.address << "\n"
         << "UART\n"
         << "  Parity: " << to_string(conf.uart_parity) << "\n"
         << "  Rate: " << to_string(conf.uart_rate) << "\n"
         << "Air Rate: " << to_string(conf.air_rate) << "\n"
         << "Channel: " << static_cast<int>(conf.channel)
            << " (" << 410 + conf.channel << "MHz)\n"
         << "Transmission Mode: "
            << (conf.transparent_transmission ? "transparent" : "fixed") << "\n"
         << "IO Drive Mode: " << to_string(conf.io_drive_mode) << "\n"
         << "Wireless Wake-up Time: " << 250 * (1 + conf.wireless_wake_up_time) << "ms\n"
         << "Error Correction: "
            << (conf.error_correction_enabled ? "enabled": "disabled") << "\n"
         << "Transmission Power: " << to_string(conf.transmission_power) << "\n";
}

template<typename T> T from_string(string const& value);

#define CASE_FROM_STRING(prefix, s) \
    if (value == #s) { \
        return Configuration::prefix ## s; \
    }

template<>
Configuration::UARTParity from_string(string const& value) {
    CASE_FROM_STRING(PARITY_, 8N1);
    CASE_FROM_STRING(PARITY_, 8O1);
    CASE_FROM_STRING(PARITY_, 8E1);

    throw std::invalid_argument(
        "invalid value for uart-parity, expected one of "
        "8N1, 8O1 or 8E1"
    );
}

template<>
Configuration::UARTRate from_string(string const& value) {
    CASE_FROM_STRING(RATE_, 1200);
    CASE_FROM_STRING(RATE_, 2400);
    CASE_FROM_STRING(RATE_, 4800);
    CASE_FROM_STRING(RATE_, 9600);
    CASE_FROM_STRING(RATE_, 19200);
    CASE_FROM_STRING(RATE_, 38400);
    CASE_FROM_STRING(RATE_, 57600);
    CASE_FROM_STRING(RATE_, 115200);

    throw std::invalid_argument(
        "invalid value for uart-rate, expected one of "
        "1200, 2400, 4800, 9600, 19200, 38400, 57600 or 115200"
    );
}

template<>
Configuration::AirRate from_string(string const& value) {
    CASE_FROM_STRING(AIR_RATE_, 300);
    CASE_FROM_STRING(AIR_RATE_, 1200);
    CASE_FROM_STRING(AIR_RATE_, 2400);
    CASE_FROM_STRING(AIR_RATE_, 4800);
    CASE_FROM_STRING(AIR_RATE_, 9600);
    CASE_FROM_STRING(AIR_RATE_, 19200);

    throw std::invalid_argument(
        "invalid value for air-rate, expected one of "
        "300, 1200, 2400, 4800, 9600 or 19200"
    );
}

template<>
Configuration::IODriveMode from_string(string const& value) {
    CASE_FROM_STRING(IO_, OPEN_DRAIN);
    CASE_FROM_STRING(IO_, PUSH_PULL);

    throw std::invalid_argument(
        "invalid value for io-drive-mode, expected one of "
        "OPEN_DRAIN or PUSH_PULL"
    );
}

template<>
Configuration::TransmissionPower from_string(string const& value) {
    CASE_FROM_STRING(POWER_, 30dBm);
    CASE_FROM_STRING(POWER_, 27dBm);
    CASE_FROM_STRING(POWER_, 24dBm);
    CASE_FROM_STRING(POWER_, 21dBm);

    throw std::invalid_argument(
        "invalid value for transmission-power, expected one of "
        "30dBm, 27dBm, 24dBm or 21dBm"
    );
}

void conf_set(Configuration& conf, string var, string value) {
    if (var == "address") {
        conf.address = std::stoi(value);
    }
    else if (var == "uart-parity") {
        conf.uart_parity = from_string<Configuration::UARTParity>(value);
    }
    else if (var == "uart-rate") {
        conf.uart_rate = from_string<Configuration::UARTRate>(value);
    }
    else if (var == "air-rate") {
        conf.air_rate = from_string<Configuration::AirRate>(value);
    }
    else if (var == "channel") {
        conf.channel = std::stoi(value);
    }
    else if (var == "transmission-mode") {
        conf.transparent_transmission = (value == "transparent");
    }
    else if (var == "io-drive-mode") {
        conf.io_drive_mode = from_string<Configuration::IODriveMode>(value);
    }
    else if (var == "wireless-wake-up-time") {
        conf.wireless_wake_up_time = std::stoi(value);
    }
    else if (var == "error-correction") {
        conf.error_correction_enabled = (value == "enabled");
    }
    else if (var == "transmission-power") {
        conf.transmission_power =
            from_string<Configuration::TransmissionPower>(value);
    }
    else {
        throw std::invalid_argument("unexpected variable name for 'set'");
    }
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        usage(cerr);
        return 1;
    }

    string uri = argv[1];
    string cmd = argv[2];

    Driver driver;
    driver.openURI(uri);

    if (cmd == "version") {
        auto version = driver.readVersion();
        cout << "v" << static_cast<int>(version.version) << " "
             << to_string(version.frequency)
             << " feature flags: " << std::hex << static_cast<int>(version.features)
             << std::endl;
    }
    else if (cmd == "show") {
        Configuration conf = driver.readConfiguration();
        conf_show(conf);
    }
    else if (cmd == "set") {
        if (argc < 5) {
            cerr << "'set' expects two arguments";
            usage(cerr);
            return 1;
        }

        string var = argv[3];
        string value = argv[4];
        Configuration conf = driver.readConfiguration();
        conf_set(conf, var, value);
        driver.writeConfiguration(conf);
    }
    else if (cmd == "save") {
        Configuration conf = driver.readConfiguration();
        driver.writeConfiguration(conf, true);
    }

    return 0;
}
