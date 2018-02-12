#ifndef AVRSUPPORT_EMULATED_MULTIPWM_H
#define AVRSUPPORT_EMULATED_MULTIPWM_H

#include <stdint.h>

#include <portlib/digitalport.hpp>

#include <utility/array.hpp>
#include <utility/iterator.hpp>

namespace AvrSupport::Emulated {
    /**
     * An emulated multichannel PWM driver with duty cycles out of 255.
     * @tparam COUNT The number of channels
     * @tparam STEP The amount by which the duty cycles are adjusted
     */
    template<PortLib::PinIndex COUNT, uint8_t STEP>
    struct MultiPwm {
    private:
        struct Channel {
            PortLib::PinIndex pin_index;
            uint8_t level;
        };

        PortLib::DigitalPort &port;
        Utility::Array<Channel, COUNT> channels;
        Channel * selection{&channels[0]};
        uint8_t counter{0};
        bool active{true};
        
    public:
        MultiPwm( // Can't be constexpr because port is non-const
            Utility::Array<PortLib::PinIndex, COUNT> const & pins,
            Utility::Array<uint8_t,           COUNT> const & levels,
            PortLib::DigitalPort & port
        ) :
            port{port}
        {
            for (auto i : Utility::Range{COUNT})
                channels[i] = Channel{pins[i], levels[i]};
        }

        /// Set all pin directions to output
        void set_pins_out() {
            for (auto & channel : channels)
                port.set_out(channel.pin_index);
        }

        /// Set desired duty cycle for a channel, 0 to 255
        void set_level(PortLib::PinIndex index, uint8_t value) {
            channels[index].level = value;
        }
        
        /// Get the current duty cycle for a pin, 0 to 255
        uint8_t get_level(PortLib::PinIndex index) const {
            return channels[index].level;
        }
        
        /// Pull selected channel's pin high, other pins low
        void isolate_selected() {
            for (auto & channel : channels)
                port.set_at(
                    channel.pin_index,
                    &channel == selection
                );
        }

        void resume() { active = true;  }
        void pause()  { active = false; }
        /// Pause and pull all pins low
        void pause_all_low() {
            pause();
            for (auto & channel : channels)
                port.set_low(channel.pin_index);
        }

        /// Advance one tick in PWM timing
        void step() {
            if (!active) return;

            if (counter == 0) {
                // At start set zero pins low, others high
                for (auto & channel : channels)
                    port.set_at(channel.pin_index, channel.level != 0);
            } else {
                // Set pins low if count passed level
                for (auto & channel : channels)
                    if (counter > channel.level)
                        port.set_low(channel.pin_index);
            }
            
            counter++;
        }

        /// Select next channel forward
        void select_next() {
            Utility::Arithmetic::circular_increment_iterator(channels, selection);
        }

        // Adjust selected channel's duty cycle up by `STEP`
        void adjust_up()   { selection->level += STEP; }
        // Adjust selected channel's duty cycle down by `STEP`
        void adjust_down() { selection->level -= STEP; }
    };
}

#endif
