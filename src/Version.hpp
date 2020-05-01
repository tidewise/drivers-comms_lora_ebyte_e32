#ifndef COMMS_LORA_EBYTE_E32_VERSION_HPP
#define COMMS_LORA_EBYTE_E32_VERSION_HPP

namespace comms_lora_ebyte_e32 {
    enum Frequency {
        FREQ_INVALID,
        FREQ_433MHZ,
        FREQ_470MHZ,
        FREQ_868MHZ,
        FREQ_915MHZ,
        FREQ_170MHZ
    };

    struct Version {
        Frequency frequency = FREQ_INVALID;
        uint8_t version = 0;
        uint8_t features = 0;
    };

    std::string to_string(Frequency f) {
        switch (f) {
            case FREQ_INVALID:
                return "invalid";
            case FREQ_433MHZ:
                return "433MHZ";
            case FREQ_470MHZ:
                return "470MHZ";
            case FREQ_868MHZ:
                return "868MHZ";
            case FREQ_915MHZ:
                return "915MHZ";
            case FREQ_170MHZ:
                return "170MHZ";
            default:
                throw std::invalid_argument("to_string: unexpected frequency");
        }
    }
}

#endif