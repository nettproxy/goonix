#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
unsigned char keyboard_scancode_to_ascii(unsigned char scancode);
unsigned char keyboard_get_key(void);

#endif // KEYBOARD_H
