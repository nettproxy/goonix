#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

namespace driver {
namespace keyboard {

class KeyboardDriver {
public:
    static void init();
    static unsigned char get_key();
    
private:
    static constexpr unsigned char KEYBOARD_MAP[] = {
        0,  27, '1', '2', '3', '4', '5', '6',
        '7', '8', '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
        'o', 'p', '[', ']', '\n', 0,  'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
        '\'', '`', 0,  '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0,  '*',
        0,  ' ', 0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  '7',
        '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    static unsigned char shift_pressed;
    static unsigned char extended_key;

    static inline unsigned char inb(unsigned short port);
    static unsigned char scancode_to_ascii(unsigned char scancode);
    static unsigned char shift_map(unsigned char c);
};

} // namespace keyboard
} // namespace driver

#endif // KEYBOARD_HPP
