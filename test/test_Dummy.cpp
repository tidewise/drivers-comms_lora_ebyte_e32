#include <boost/test/unit_test.hpp>
#include <comms_lora_ebyte_e32/Dummy.hpp>

using namespace comms_lora_ebyte_e32;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    comms_lora_ebyte_e32::DummyClass dummy;
    dummy.welcome();
}
