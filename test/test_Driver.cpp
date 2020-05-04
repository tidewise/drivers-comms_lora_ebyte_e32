#include <gtest/gtest.h>
#include <comms_lora_ebyte_e32/Driver.hpp>

using namespace comms_lora_ebyte_e32;

struct DriverTest : public ::testing::Test {
    Driver driver;
};

TEST_F(DriverTest, it_encodes_a_configuration_structure) {
    Configuration config;
    config.address = 0x0102;
    config.uart_parity = Configuration::PARITY_8O1;
    config.uart_rate = Configuration::RATE_19200;
    config.air_rate = Configuration::AIR_RATE_4800;
    config.channel = 20;
    config.transparent_transmission = false;
    config.io_drive_mode = Configuration::IO_PUSH_PULL;
    config.wireless_wake_up_time = 4;
    config.error_correction_enabled = true;
    config.transmission_power = Configuration::POWER_24dBm;

    uint8_t buffer[5];
    driver.encodeConfiguration(buffer, config);

    ASSERT_EQ(buffer[0], 0x1);
    ASSERT_EQ(buffer[1], 0x2);
    ASSERT_EQ(buffer[2], 0b01100011);
    ASSERT_EQ(buffer[3], 0b00010100);
    ASSERT_EQ(buffer[4], 0b11100110);
}

TEST_F(DriverTest, it_decodes_a_configuration_structure) {

    uint8_t buffer[5] = { 0x1, 0x2, 0b01100011, 0b00010100, 0b11100110 };
    auto config = driver.decodeConfiguration(buffer);

    ASSERT_EQ(0x0102, config.address);
    ASSERT_EQ(Configuration::PARITY_8O1, config.uart_parity);
    ASSERT_EQ(Configuration::RATE_19200, config.uart_rate);
    ASSERT_EQ(Configuration::AIR_RATE_4800, config.air_rate);
    ASSERT_EQ(20, config.channel);
    ASSERT_FALSE(config.transparent_transmission);
    ASSERT_EQ(Configuration::IO_PUSH_PULL, config.io_drive_mode);
    ASSERT_EQ(4, config.wireless_wake_up_time);
    ASSERT_TRUE(config.error_correction_enabled);
    ASSERT_EQ(Configuration::POWER_24dBm, config.transmission_power);
}

TEST_F(DriverTest, decoding_handles_special_parity_value) {
    uint8_t buffer[5] = { 0, 0, 0b11000000, 0, 0 };
    auto config = driver.decodeConfiguration(buffer);
    ASSERT_EQ(Configuration::PARITY_8N1, config.uart_parity);
}

TEST_F(DriverTest, decoding_handles_special_air_rate_values) {
    uint8_t buffer[5] = { 0, 0, 0b00000110, 0, 0 };
    ASSERT_EQ(Configuration::AIR_RATE_19200,
              driver.decodeConfiguration(buffer).air_rate);

    buffer[2] = 0b00000111;
    ASSERT_EQ(Configuration::AIR_RATE_19200,
              driver.decodeConfiguration(buffer).air_rate);
}