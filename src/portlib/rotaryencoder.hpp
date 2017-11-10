#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

namespace AVRSupport::PortLib {
    struct RotaryEncoder {
        enum struct Active : uint8_t { A, B, BOTH, NONE };
        using Reader = bool (*)();

    private:
        struct State {
            bool a, b;
            
            constexpr State() : a(false), b(false) {}
            constexpr State(bool a, bool b) : a(a), b(b) {}

            inline bool odd() { return a != b; }
            inline bool even_on() { return a && b; }
            inline bool even_off() { return !a && !b; }
        };

        State previous, current;
        Reader read_a;
        Reader read_b;

    public:
        constexpr RotaryEncoder(Reader a, Reader b) :
            read_a(a), read_b(b) {}

        inline void update() {
            previous = current;
            current = { read_a(), read_b() };
        }

        inline bool rising_edge() {
            return previous.even_off() && current.odd();
        }

        inline bool falling_edge() {
            return previous.even_on() && current.odd();
        }

        inline Active get_active() {
            if (current.a) {
                if (current.b) return Active::BOTH;
                else           return Active::A;
            } else {
                if (current.b) return Active::B;
                else           return Active::NONE;
            }
        }
    };
}

#endif
