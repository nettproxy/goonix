#include "shell.h"
#include "../lib/video.h"
#include "../lib/video.c"
#include "../driver/keyboard/keyboard.c"
#include <stdint.h>
#include "../lib/multiboot.h"



/*           commands                 */

#include "../commands/clear/clear.c"
#include "../commands/hello/hello.c"
#include "../commands/reboot/reboot.c"
#include "../commands/meminfo/meminfo.c"
#include "../commands/halt/halt.c"

/*           code                     */

static int string_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}


void video_print_number(uint32_t num) {
    char buf[32];
    int i = 0;

    if (num == 0) {
        video_print_string("0");
        return;
    }

    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i--) {
        char c[2] = {buf[i], 0};
        video_print_string(c);
    }
}

uint64_t total_memory = 0;
uint64_t free_memory = 0;

void parse_memory(multiboot_info_t *mb) {
    if (!(mb->flags & (1 << 6))) {
        return; // no mmap
    }

    uintptr_t addr = mb->mmap_addr;
    uintptr_t end  = addr + mb->mmap_length;

    while (addr < end) {
        multiboot_mmap_entry_t *entry =
            (multiboot_mmap_entry_t *)addr;

        if (entry->type == 1) {
            free_memory += entry->len;
        }

        total_memory += entry->len;

        addr += entry->size + sizeof(entry->size);
    }
}

static void process_command(const char *cmd) {
    if (string_equals(cmd, "clear")) {
        command_clear();
        return;
    }
    if (string_equals(cmd, "hello")) {
        command_hello();
        return;
    }
    if (string_equals(cmd, "reboot")) {
        command_reboot();
        return;
    }
    if (string_equals(cmd, "halt")) {
        command_halt();
        return;
    }
    if (string_equals(cmd, "meminfo")) {
        command_meminfo();
        return;
    }
    if (*cmd != '\0') {
        video_print_string("Gng ts command was NOT found\n");
    }
}

static void print_prompt(void) {
    video_print_string("> ");
}

void shell_init(unsigned int addr) {
    mb_info = (struct multiboot_info *)addr;
    video_init();
    keyboard_init();
}

void shell_run(void) {
    video_print_string("   ___               _     \n  / __|___  ___ _ _ (_)_ __\n | (_ / _ \/ _ \\ ' \\| \\ \\ /\n  \\___\\___/\\___/_||_|_/\\_\\\n                           \n");
    video_print_string("Developed by Carti from scratch");
    video_print_string("\n---------------------------------\n");
    video_print_string("[github.com/nettproxy/Goonix]   \n\n");
    print_prompt();

    int line_start = video_get_cursor_pos();

    char buffer[64];
    int length = 0;
    int buffer_cursor = 0;
    int blink_counter = 0;
    int cursor_visible = 1;

    while (1) {
        blink_counter++;
        if (blink_counter > 50000) {
            blink_counter = 0;
            cursor_visible = !cursor_visible;
            video_draw_cursor(cursor_visible);
        }

        unsigned char key = keyboard_get_key();
        if (key == 0) {
            continue;
        }

        cursor_visible = 1;
        video_draw_cursor(0);
        blink_counter = 0;

        if (key == '\n') {
            video_put_char('\n');
            buffer[length] = '\0';
            process_command(buffer);
            length = 0;
            buffer_cursor = 0;
            if (video_get_cursor_pos() >= 80 * 25) {
                video_clear_screen();
            }
            print_prompt();
            line_start = video_get_cursor_pos();
            continue;
        }

        if (key == 0x80) {
            if (buffer_cursor > 0) {
                buffer_cursor--;
                video_set_cursor_pos(video_get_cursor_pos() - 1);
                video_draw_cursor(1);
            }
            continue;
        }

        if (key == 0x81) {
            if (buffer_cursor < length) {
                buffer_cursor++;
                video_set_cursor_pos(video_get_cursor_pos() + 1);
                video_draw_cursor(1);
            }
            continue;
        }

        if (key == '\b') {
            if (buffer_cursor > 0) {
                buffer_cursor--;
                length--;
                for (int i = buffer_cursor; i < length; i++) {
                    buffer[i] = buffer[i + 1];
                }
                video_redraw_line(line_start, buffer, length, buffer_cursor);
            }
            continue;
        }

        if (length < (int)sizeof(buffer) - 1) {
            for (int i = length; i > buffer_cursor; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[buffer_cursor] = (char)key;
            length++;
            buffer_cursor++;
            
            if (buffer_cursor == length) {
                video_put_char((char)key);
            } else {
                video_redraw_line(line_start, buffer, length, buffer_cursor);
            }
        }
    }
}
