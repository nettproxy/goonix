#include "../driver/keyboard/keyboard.c"
#include "../commands/clear/clear.c"

static unsigned short *video_memory = (unsigned short*)0xB8000;
static int cursor_position = 0;

void clear_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (unsigned short)(' ') | ((unsigned short)0x07 << 8);
    }
    cursor_position = 0;
}

static void move_cursor_newline(void) {
    cursor_position += 80 - (cursor_position % 80);
    if (cursor_position >= 80 * 25) {
        clear_screen();
    }
}

static void put_char(char c) {
    if (c == '\n') {
        move_cursor_newline();
        return;
    }

    if (cursor_position >= 80 * 25) {
        clear_screen();
    }

    video_memory[cursor_position++] = (unsigned short)c | ((unsigned short)0x07 << 8);
}

static void print_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

static void print_prompt(void) {
    print_string("> ");
}

static int string_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

static void process_command(const char *cmd) {
    if (string_equals(cmd, "clear")) {
        command_clear();
        return;
    }
    if (*cmd != '\0') {
        print_string("Unknown command\n");
    }
}

void kernel_main(void) {
    clear_screen();
    keyboard_init();
    print_string("Goonix\n");
    print_prompt();

    char buffer[64];
    int length = 0;

    while (1) {
        unsigned char key = keyboard_get_key();
        if (key == 0) {
            continue;
        }

        if (key == '\n') {
            put_char('\n');
            buffer[length] = '\0';
            process_command(buffer);
            length = 0;
            if (cursor_position >= 80 * 25) {
                clear_screen();
            }
            print_prompt();
            continue;
        }

        if (key == '\b') {
            if (length > 0) {
                length--;
                put_char('\b');
            }
            continue;
        }

        if (length < (int)sizeof(buffer) - 1) {
            buffer[length++] = (char)key;
            put_char((char)key);
        }
    }
}
