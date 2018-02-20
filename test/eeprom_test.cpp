#define private public

#include <portlib/register.hpp>
#include <portlib/eeprom.hpp>
#include <utility/stddef.hpp>

#include <cstdint>
#include <cassert>

using namespace AvrSupport;

Utility::avr_size_t const EEPROM_SIZE{512};

uint8_t EEDR, EECR;
uint16_t EEAR;

struct MyStruct {
    uint8_t * thingo;
    uint8_t thingy;
};

using Eeprom = PortLib::Eeprom<EEPROM_SIZE>;
using BufferEeprom = PortLib::BufferEeprom<EEPROM_SIZE>;
template<typename Type>
using ValueEeprom = PortLib::ValueEeprom<EEPROM_SIZE, Type>;

template<typename Enum>
void assert_enum(Enum e, uint8_t v) {
    assert(static_cast<uint8_t>(e) == v);
}

void control_mask_test() {
    assert_enum(Eeprom::ControlMask::read, 0b1);
    assert_enum(Eeprom::ControlMask::write, 0b10);
    assert_enum(Eeprom::ControlMask::master_write, 0b100);
    assert_enum(Eeprom::ControlMask::interrupt, 0b1000);
}

void buffer_eeprom_test() {
    BufferEeprom eeprom{EEDR, EECR, EEAR};
}

void value_eeprom_test() {
    ValueEeprom<MyStruct> eeprom{EEDR, EECR, EEAR};
}

int main() {
    control_mask_test();
    buffer_eeprom_test();
    value_eeprom_test();
    return 0;
}
