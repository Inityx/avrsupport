#ifndef AVRSUPPORT_EMULATED_PWM_H
#define AVRSUPPORT_EMULATED_PWM_H

#include <stdint.h>

#include <portlib/digitalpin.hpp>

namespace avrsupport::emulated {
    /// An emulated single channel PWM driver with duty cycles out of 255.
    struct Pwm {
    private:
        uint8_t counter{0};
    
    public:
        uint8_t level;
        bool active{true};
        portlib::DigitalPin & pin;

        Pwm(portlib::DigitalPin & pin, uint8_t const level)
            : pin{pin}, level{level} {}

        /// Advance one tick in PWM timing
        void step() {
            if (!active) return;

            if (counter == 0) {
                // Set high if level > zero, low otherwise
                pin.set(level != 0);
            } else if (counter > level) {
                // Set low if counter passed level
                pin.set_low();
            }

            counter++;
        }
    };
}

#endif
