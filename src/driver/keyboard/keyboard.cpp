#include "keyboard.hpp"

namespace driver {
namespace keyboard {

unsigned char KeyboardDriver::shift_pressed = 0;
unsigned char KeyboardDriver::extended_key = 0;

inline unsigned char KeyboardDriver::inb(unsigned short port) {
    unsigned char value;
    asm volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

unsigned char KeyboardDriver::shift_map(unsigned char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }

    switch (c) {
        case '1': return '!';
        case '2': return '"';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '&';
        case '7': return '/';
        case '8': return '(';
        case '9': return ')';
        case '0': return '=';
        case '-': return '?';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        case '`': return '~';
        case '\\': return '|';
        default: return c;
    }
}

unsigned char KeyboardDriver::scancode_to_ascii(unsigned char scancode) {
    if (scancode < sizeof(KEYBOARD_MAP)) {
        unsigned char c = KEYBOARD_MAP[scancode];
        if (shift_pressed) {
            return shift_map(c);
        }
        return c;
    }
    return 0;
}

void KeyboardDriver::init() {
    (void)KEYBOARD_MAP; // This function is useless btw
}

unsigned char KeyboardDriver::get_key() {
    if (!(inb(0x64) & 1)) {
        return 0;
    }

    unsigned char scancode = inb(0x60);
    
    if (scancode == 0xE0) {
        extended_key = 1;
        return 0;
    }
    
    if (extended_key) {
        extended_key = 0;
        if (scancode == 0x4B) return 0x80;
        if (scancode == 0x4D) return 0x81;
        if (scancode == 0x48) return 0x82;
        if (scancode == 0x50) return 0x83;
        return 0;
    }
    
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return 0;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return 0;
    }
    if (scancode & 0x80) {
        return 0;
    }

    return scancode_to_ascii(scancode);
}

} // namespace keyboard
} // namespace driver
