#ifndef MULTIPWM_H
#define MULTIPWM_H

#include <stdint.h>
#include <stddef.h>

#include "../portlib/portlib.hpp"
#include "../portlib/digitalport.hpp"

#include "../utility/array.hpp"

namespace AvrSupport::Emulated {
    using PortLib::DigitalPort;
    using PortLib::PinIndex;
    using Utility::Array;

    template<PinIndex COUNT, uint8_t STEP>
    struct MultiPWM {
    private:
        Array<PinIndex, COUNT> pins;
        Array<uint8_t, COUNT> levels;
        DigitalPort &port;
        uint8_t counter;
        
    public:
        bool active;
        PinIndex selection;
        
        constexpr MultiPWM(
            Array<PinIndex, COUNT> const &pins,
            Array<uint8_t, COUNT> const &levels,
            DigitalPort &port
        ) :
            pins{pins},
            levels{levels},
            port{port},
            counter{0},
            active{true},
            selection{0}
        {}

        void set_level(PinIndex index, uint8_t value) {
            levels[index] = value;
        }
        
        void isolate_selected() {
            for (uint8_t i{0}; i<COUNT; i++)
                if (i != selection)
                    port.set_low(pins[i]);

            port.set_high(pins[selection]);
        }

        void step() {
            if (!active) return;

            if (counter > 0) {
                // Set pins low if count passed level
                for (PinIndex i{0}; i<COUNT; i++)
                    if (counter > levels[i])
                        port.set_low(pins[i]);
            } else {
                // Set all pins high at start
                for (PinIndex i{0}; i<COUNT; i++)
                    if (levels[i] != 0)
                        port.set_high(pins[i]);
            }
            
            counter++;
        }

        void select_next() {
            if (selection < COUNT-1) selection++;
            else selection = 0;
        }

        void adjust_up()   { levels[selection] += STEP; }
        void adjust_down() { levels[selection] -= STEP; }
    };
}

#endif
