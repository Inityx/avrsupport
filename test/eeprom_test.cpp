#include "portlib/portlib.hpp"
#include "portlib/eeprom.hpp"

#include <stdint.h>

using namespace AvrSupport;

PortLib::eeprom_size_t const EEPROM_SIZE{512};

uint8_t EEDR, EECR;
uint16_t EEAR;

struct MyStruct {
    uint8_t * thingo; uint8_t thingy;
};

int buffer_eeprom_test() {
    PortLib::BufferEeprom<EEPROM_SIZE> eeprom{EEDR, EECR, EEAR};
    return 0;
}

int value_eeprom_test() {
    PortLib::ValueEeprom<EEPROM_SIZE, MyStruct> eeprom{EEDR, EECR, EEAR};
    return 0;
}

int main() {
    return (
        buffer_eeprom_test() ||
        value_eeprom_test()
    );
}