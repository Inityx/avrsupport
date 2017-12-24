#ifndef MULTIPWM_H
#define MULTIPWM_H

#include <stdint.h>
#include <stddef.h>

#include <portlib/register.hpp>
#include <portlib/digitalport.hpp>

#include <utility/array.hpp>

namespace AvrSupport::Emulated {
    template<PortLib::PinIndex COUNT, uint8_t STEP>
    struct MultiPWM {
    private:
        PortLib::DigitalPort &port;
        Utility::Array<PortLib::PinIndex, COUNT> pins;
        Utility::Array<uint8_t, COUNT> levels;
        PortLib::PinIndex selection;
        uint8_t counter;
        bool active;
        
    public:
        constexpr MultiPWM(
            Utility::Array<PortLib::PinIndex, COUNT> const &pins,
            Utility::Array<uint8_t, COUNT> const &levels,
            PortLib::DigitalPort &port
        ) :
            pins{pins},
            levels{levels},
            port{port},
            counter{0},
            active{true},
            selection{0}
        {}

        void set_pins_out() {
            for (PortLib::PinIndex i : pins)
                port.set_out(i);
        }

        void set_level(PortLib::PinIndex index, uint8_t value) {
            levels[index] = value;
        }
        
        void isolate_selected() {
            for (uint8_t i{0}; i<COUNT; i++)
                if (i != selection)
                    port.set_low(pins[i]);

            port.set_high(pins[selection]);
        }

        void pause() { active = false; }
        void resume() { active = true; }

        void step() {
            if (!active) return;

            if (counter > 0) {
                // Set pins low if count passed level
                for (PortLib::PinIndex i{0}; i<COUNT; i++)
                    if (counter > levels[i])
                        port.set_low(pins[i]);
            } else {
                // Set all pins high at start
                for (PortLib::PinIndex i{0}; i<COUNT; i++)
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
