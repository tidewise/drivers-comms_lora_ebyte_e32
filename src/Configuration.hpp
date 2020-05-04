#ifndef COMMS_LORA_EBYTE_E32_CONFIGURATION_HPP
#define COMMS_LORA_EBYTE_E32_CONFIGURATION_HPP

namespace comms_lora_ebyte_e32 {
    struct Configuration {
        uint16_t address;

        enum UARTParity {
            PARITY_8N1,
            PARITY_8O1,
            PARITY_8E1
        };
        UARTParity uart_parity = PARITY_8N1;

        enum UARTRate {
            RATE_1200,
            RATE_2400,
            RATE_4800,
            RATE_9600,
            RATE_19200,
            RATE_38400,
            RATE_57600,
            RATE_115200
        };
        UARTRate uart_rate = RATE_9600;

        enum AirRate {
            AIR_RATE_300,
            AIR_RATE_1200,
            AIR_RATE_2400,
            AIR_RATE_4800,
            AIR_RATE_9600,
            AIR_RATE_19200
        };
        AirRate air_rate = AIR_RATE_2400;

        /** Channel, from 0x00 to 0x1f
         *
         * Frequency = 410M + channel * 1M
         */
        uint8_t channel = 0;

        /** Transparent or fixed transmission modes
         *
         * In transparent mode, the modem transparently sends
         * all bytes received on the UART through the air.
         *
         * In fixed mode, the first three bytes are used as
         * high/low address and channel
         */
        bool transparent_transmission = true;

        enum IODriveMode {
            IO_OPEN_DRAIN,
            IO_PUSH_PULL
        };

        /** I/O pins drive mode
         *
         * If false, TXD and AUX are push-pull outputs, RXD is pull-up
         * If true, TXD, AUX and RXD are all open collectors
         */
        IODriveMode io_drive_mode = IO_PUSH_PULL;

        /** Wireless wake-up time as 250ms + wake_up_time * 250ms
         *
         * Max value is 2s (7). Default is 250ms (0)
         */
        uint8_t wireless_wake_up_time = 0;

        /** Whether error correction is enabled
         *
         * Both sides need to have the same setting. Disabling it augments data
         * rate in short-range, low-interference environments, but has reduced
         * usable range and lower robustness
         */
        bool error_correction_enabled = true;

        enum TransmissionPower {
            POWER_30dBm,
            POWER_27dBm,
            POWER_24dBm,
            POWER_21dBm
        };

        TransmissionPower transmission_power = POWER_30dBm;
    };
}

#endif