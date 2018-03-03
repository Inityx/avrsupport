#include <cassert>

#include <portlib/digitalport.hpp>

using avrsupport::portlib::DigitalPort;

volatile uint8_t PINA{0b110};
volatile uint8_t PORTA{0};
volatile uint8_t DDRA{0};

DigitalPort port_a{ PINA, PORTA, DDRA };

int main() {
    // Set direction
    port_a.set_out(0);
    port_a.set_out(1);
    port_a.set_out(2);
    
    assert(DDRA == 0b111);
    
    // Set value
    port_a.set_high(0);
    port_a.set_low(1);
    port_a.set_high(2);

    assert(PORTA == 0b101);

    // Read value
    assert(port_a.get(0) == false);
    assert(port_a.get(1) == true);
    assert(port_a.get(2) == true);

    return 0;
}
