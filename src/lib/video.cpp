#include "video.hpp"

namespace lib {
namespace video {

static unsigned short *video_memory = (unsigned short*)0xB8000;
static int cursor_position = 0;
static unsigned char video_color = 0x07;

static void scroll_up() {
    // Scroll all lines up by one
    for (int i = 0; i < 80 * 24; i++) {
        video_memory[i] = video_memory[i + 80];
    }
    // Clear the bottom line
    for (int i = 80 * 24; i < 80 * 25; i++) {
        video_memory[i] = (unsigned short)(' ') | ((unsigned short)video_color << 8);
    }
    cursor_position -= 80;
}

unsigned char get_color() {
    return video_color;
}

void set_color(unsigned char fg, unsigned char bg) {
    video_color = (unsigned char)((bg << 4) | (fg & 0x0F));
}

void set_color_attr(unsigned char attr) {
    video_color = attr;
}

void init() {
    clear_screen();
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (unsigned short)(' ') | ((unsigned short)video_color << 8);
    }
    cursor_position = 0;
}

static void move_cursor_newline() {
    cursor_position += 80 - (cursor_position % 80);
    if (cursor_position >= 80 * 25) {
        scroll_up();
    }
}

void put_char(char c) {
    if (c == '\n') {
        move_cursor_newline();
        return;
    }

    if (cursor_position >= 80 * 25) {
        scroll_up();
    }

    video_memory[cursor_position++] = (unsigned short)c | ((unsigned short)video_color << 8);
}

void erase_char() {
    if (cursor_position > 0) {
        cursor_position--;
        video_memory[cursor_position] = (unsigned short)(' ') | ((unsigned short)video_color << 8);
    }
}

void draw_cursor(int visible) {
    video_memory[cursor_position] = (unsigned short)(visible ? '_' : ' ') | ((unsigned short)video_color << 8);
}

void print_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

void redraw_line(int line_start, const char *buffer, int length, int cursor_pos) {
    draw_cursor(0);
    
    for (int i = 0; i < length; i++) {
        video_memory[line_start + i] = (unsigned short)buffer[i] | ((unsigned short)video_color << 8);
    }
    for (int i = length; i < 64; i++) {
        video_memory[line_start + i] = (unsigned short)(' ') | ((unsigned short)video_color << 8);
    }
    cursor_position = line_start + cursor_pos;
    draw_cursor(1);
}

int get_cursor_pos() {
    return cursor_position;
}

void set_cursor_pos(int pos) {
    cursor_position = pos;
}

} // namespace video
} // namespace lib
