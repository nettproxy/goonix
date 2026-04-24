#include "keyboard.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    asm volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

static unsigned char shift_pressed = 0;

static const unsigned char keyboard_map[] = {
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

void keyboard_init(void) {
    (void)keyboard_map;
}

static unsigned char shift_map(unsigned char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }

    switch (c) {
        case '1': return '!';
        case '2': return '"';
        case '3': return '§';
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

unsigned char keyboard_scancode_to_ascii(unsigned char scancode) {
    if (scancode < sizeof(keyboard_map)) {
        unsigned char c = keyboard_map[scancode];
        if (shift_pressed) {
            return shift_map(c);
        }
        return c;
    }
    return 0;
}

unsigned char keyboard_get_key(void) {
    if (!(inb(0x64) & 1)) {
        return 0;
    }

    unsigned char scancode = inb(0x60);
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

    return keyboard_scancode_to_ascii(scancode);
}
