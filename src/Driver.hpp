#ifndef COMMS_LORA_EBYTE_E32_DRIVER_HPP
#define COMMS_LORA_EBYTE_E32_DRIVER_HPP

#include <iodrivers_base/Driver.hpp>
#include <comms_lora_ebyte_e32/Configuration.hpp>
#include <comms_lora_ebyte_e32/Version.hpp>

namespace comms_lora_ebyte_e32 {
    /**
     * Driver for the EByte E32 module
     *
     * This driver does not deal with mode switching. Relevant methods must be
     * called while the module is already in the right mode
     *
     * Reading should be done with readRaw() with appropriate timeouts. The driver
     * provides \c writeRaw to send data to a particular target & channel
     */
    class Driver : public iodrivers_base::Driver {
        int extractPacket(uint8_t const* buffer, size_t buffer_size) const;
    public:
        Driver();

        /** Decode the configuration raw representation
         *
         * @param buffer the configuration part of the buffer, without the first
         *    byte of the board's reply. It is expected to be 5 bytes long.
         */
        static Configuration decodeConfiguration(uint8_t const* buffer);

        /** Decode the configuration raw representation
         *
         * @param buffer the configuration part of the buffer, without the first
         *    byte of the board's reply. 5 bytes will be written to it.
         */
        static void encodeConfiguration(uint8_t* buffer, Configuration const& conf);

        /** Read the configuration from the board */
        Configuration readConfiguration();

        /** Write a new configuration to the board */
        void writeConfiguration(Configuration const& conf, bool save = false);

        /** @overload
        *
        * Calls readRaw using the default timeout as packet timeout
        */
        int writeRaw(uint16_t target, uint8_t channel, uint8_t* buffer, int bufsize);

        /** @overload
        *
        * Use the same timeout for first byte and packet
        */
        int writeRaw(uint16_t target, uint8_t channel,
                     uint8_t* buffer, int bufsize, base::Time const& timeout);

        /** Read raw bytes from the underlying I/O
        *
        * Reads as many bytes as possible within the time boundaries specified
        * by its timeout parameters, not attempting to extract packets
        *
        * @arg packet_timeout the overall timeout. The method will return at most
        *   after that much time has elapsed
        * @arg first_byte_timeout return if no bytes are received within that
        *   much time
        * @arg inter_byte_timeout return if no new bytes have been received after
        *   that much time has elapsed since the last received byte
        *
        * This never throws
        */
        int writeRaw(uint16_t target, uint8_t channel, uint8_t* buffer, int bufsize,
                     base::Time const& packet_timeout,
                     base::Time const& first_byte_timeout,
                     base::Time const& inter_byte_timeout = base::Time());

        Version readVersion();
    };
}

#endif