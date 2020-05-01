rock_library(comms_lora_ebyte_e32
    SOURCES Driver.cpp
    HEADERS Driver.hpp Version.hpp
    DEPS_PKGCONFIG iodrivers_base)

rock_executable(comms_lora_ebyte_e32_bin Main.cpp
    DEPS comms_lora_ebyte_e32)
