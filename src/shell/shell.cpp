#include "shell.hpp"
#include "../lib/video.hpp"
#include "../lib/video.cpp"
#include "../lib/utils/utils.hpp"
#include "../lib/utils/utils.cpp"
#include "../driver/keyboard/keyboard.cpp"
#include "../driver/graphics/graphics.cpp"
#include <stdint.h>
#include "../lib/multiboot.h"
#include "../commands/command.hpp"

/*           commands                 */

#include "../commands/clear/clear.cpp"
#include "../commands/hello/hello.cpp"
#include "../commands/reboot/reboot.cpp"
#include "../commands/meminfo/meminfo.cpp"
#include "../commands/halt/halt.cpp"
#include "../commands/clrtest/clrtest.cpp"
#include "../commands/random/random.cpp"
#include "../commands/info/info.cpp"
#include "../commands/help/help.cpp"
#include "../commands/echo/echo.cpp"
#include "../commands/commands.cpp"

/*           code                     */

using namespace driver::keyboard;
using namespace lib::video;
using namespace lib::utils;

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
    if (*cmd != '\0') {
        char name_buf[64];
        int name_len = 0;

        const char *p = cmd;
        while (*p == ' ') p++;
        while (*p && *p != ' ' && name_len < (int)sizeof(name_buf) - 1) {
            name_buf[name_len++] = *p++;
        }
        name_buf[name_len] = '\0';

        while (*p == ' ') p++;
        const char *args = (*p != '\0') ? p : 0;

        for (int i = 0; commands[i].name; i++) {
            if (string_equals(name_buf, commands[i].name)) {
                commands[i].fn(args);
                return;
            }
        }

        const char *best = 0;
        int best_dist = 1000000;
        for (int i = 0; commands[i].name; i++) {
            int d = levenshtein_distance(name_buf, commands[i].name);
            if (d < best_dist) {
                best_dist = d;
                best = commands[i].name;
            }
        }

        if (best && best_dist <= 2) {
            video_print_string("Command not found. Did you mean '");
            video_print_string(best);
            video_print_string("'?\n");
        } else {
            video_print_string("Command not found.\n");
        }
    }
}

static void print_prompt(void) {
    unsigned char old = video_get_color();

    video_set_color(VIDEO_COLOR_LIGHT_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("root");
    video_set_color(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK);
    video_print_string("@");
    video_set_color(VIDEO_COLOR_LIGHT_GREEN, VIDEO_COLOR_BLACK);
    video_print_string("Goonix");
    video_set_color(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK);
    video_print_string(" # ");

    video_set_color_attr(old);
}

void shell_init(unsigned int addr) {
    mb_info = (struct multiboot_info *)addr;
    video_init();
    KeyboardDriver::init();
    driver::graphics::GraphicsDriver::init();
}

void shell_run(void) {
    video_set_color(VIDEO_COLOR_LIGHT_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("   ___               _     \n  / __|___  ___ _ _ (_)_ __\n | (_ / _ \\/ _ \\\\ ' \\| \\\\ \\\\ /\n  \\\\___\\\\___/\\\\___/_||_|_/\\\\_\\\\\n                           \n");
    video_set_color_attr(VIDEO_COLOR_WHITE);
    video_set_color(VIDEO_COLOR_LIGHT_GREEN, VIDEO_COLOR_BLACK);
    video_print_string("Developed by Carti from scratch");
    video_set_color_attr(VIDEO_COLOR_WHITE);
    video_print_string("\n---------------------------------\n");
    video_set_color_attr(VIDEO_COLOR_CYAN);
    video_set_color(VIDEO_COLOR_LIGHT_GREEN, VIDEO_COLOR_BLACK);
    video_print_string("[github.com/nettproxy/Goonix]   \n\n");
    video_set_color_attr(VIDEO_COLOR_WHITE);
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

        unsigned char key = KeyboardDriver::get_key();
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
