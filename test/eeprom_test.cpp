#include "portlib/register.hpp"
#include "portlib/eeprom.hpp"

#include <cstdint>
#include <cassert>

using namespace AvrSupport;

PortLib::eeprom_size_t const EEPROM_SIZE{512};

uint8_t EEDR, EECR;
uint16_t EEAR;

struct MyStruct {
    uint8_t * thingo;
    uint8_t thingy;
};

void buffer_eeprom_test() {
    PortLib::BufferEeprom<EEPROM_SIZE> eeprom{EEDR, EECR, EEAR};
}

void value_eeprom_test() {
    PortLib::ValueEeprom<EEPROM_SIZE, MyStruct> eeprom{EEDR, EECR, EEAR};
}

int main() {
    buffer_eeprom_test();
    value_eeprom_test();
    return 0;
}