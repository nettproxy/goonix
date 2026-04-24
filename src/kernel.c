static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    asm volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

static unsigned short *video_memory = (unsigned short*)0xB8000;
static int cursor_position = 0;

static void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (unsigned short)(' ') | ((unsigned short)0x07 << 8);
    }
    cursor_position = 0;
}

static void move_cursor_newline() {
    cursor_position += 80 - (cursor_position % 80);
    if (cursor_position >= 80 * 25) {
        cursor_position = 0;
    }
}

static void put_char(char c) {
    if (c == '\n') {
        move_cursor_newline();
        return;
    }

    video_memory[cursor_position++] = (unsigned short)c | ((unsigned short)0x07 << 8);
    if (cursor_position >= 80 * 25) {
        cursor_position = 0;
    }
}

static void print_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

static void print_decimal(unsigned int value) {
    char buffer[12];
    int pos = 0;

    if (value == 0) {
        put_char('0');
        return;
    }

    while (value > 0 && pos < (int)sizeof(buffer) - 1) {
        buffer[pos++] = '0' + (value % 10);
        value /= 10;
    }

    while (pos > 0) {
        put_char(buffer[--pos]);
    }
}

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
    0,  0,  0,  0,  0,  0,  0,  0
};

void kernel_main() {
    clear_screen();
    while (1) {
        unsigned char status = inb(0x64);
        if (status & 1) {
            unsigned char scancode = inb(0x60);
            if ((scancode & 0x80) == 0) {
                unsigned char ascii = 0;
                if (scancode < 128) {
                    ascii = keyboard_map[scancode];
                }

                print_string("Scan: ");
                print_decimal(scancode);
                print_string("   ASCII: ");

                if (ascii >= ' ' && ascii <= '~') {
                    print_decimal((unsigned int)ascii);
                    print_string(" ('");
                    put_char((char)ascii);
                    print_string("')\n");
                } else if (ascii == '\b') {
                    print_string("8 (BACKSPACE)\n");
                } else if (ascii == '\t') {
                    print_string("9 (TAB)\n");
                } else if (ascii == '\n') {
                    print_string("10 (ENTER)\n");
                } else if (ascii == 27) {
                    print_string("27 (ESC)\n");
                } else if (ascii == 0) {
                    print_string("0 (no ASCII mapping)\n");
                } else {
                    print_string("?\n");
                }
            }
        }
    }
}
