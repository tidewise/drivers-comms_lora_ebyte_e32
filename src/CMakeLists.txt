rock_library(comms_lora_ebyte_e32
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(comms_lora_ebyte_e32_bin Main.cpp
    DEPS comms_lora_ebyte_e32)

