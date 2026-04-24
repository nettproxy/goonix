#include "video.h"

static unsigned short *video_memory = (unsigned short*)0xB8000;
static int cursor_position = 0;

void video_init(void) {
    video_clear_screen();
}

void video_clear_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (unsigned short)(' ') | ((unsigned short)0x07 << 8);
    }
    cursor_position = 0;
}

static void move_cursor_newline(void) {
    cursor_position += 80 - (cursor_position % 80);
    if (cursor_position >= 80 * 25) {
        video_clear_screen();
    }
}

void video_put_char(char c) {
    if (c == '\n') {
        move_cursor_newline();
        return;
    }

    if (cursor_position >= 80 * 25) {
        video_clear_screen();
    }

    video_memory[cursor_position++] = (unsigned short)c | ((unsigned short)0x07 << 8);
}

void video_erase_char(void) {
    if (cursor_position > 0) {
        cursor_position--;
        video_memory[cursor_position] = (unsigned short)(' ') | ((unsigned short)0x07 << 8);
    }
}

void video_draw_cursor(int visible) {
    video_memory[cursor_position] = (unsigned short)(visible ? '_' : ' ') | ((unsigned short)0x07 << 8);
}

void video_print_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        video_put_char(str[i]);
    }
}

void video_redraw_line(int line_start, const char *buffer, int length, int cursor_pos) {
    video_draw_cursor(0);
    
    for (int i = 0; i < length; i++) {
        video_memory[line_start + i] = (unsigned short)buffer[i] | ((unsigned short)0x07 << 8);
    }
    for (int i = length; i < 64; i++) {
        video_memory[line_start + i] = (unsigned short)(' ') | ((unsigned short)0x07 << 8);
    }
    cursor_position = line_start + cursor_pos;
    video_draw_cursor(1);
}

int video_get_cursor_pos(void) {
    return cursor_position;
}

void video_set_cursor_pos(int pos) {
    cursor_position = pos;
}
