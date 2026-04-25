#ifndef VIDEO_H
#define VIDEO_H

enum video_color {
    VIDEO_COLOR_BLACK = 0,
    VIDEO_COLOR_BLUE = 1,
    VIDEO_COLOR_GREEN = 2,
    VIDEO_COLOR_CYAN = 3,
    VIDEO_COLOR_RED = 4,
    VIDEO_COLOR_MAGENTA = 5,
    VIDEO_COLOR_BROWN = 6,
    VIDEO_COLOR_LIGHT_GREY = 7,
    VIDEO_COLOR_DARK_GREY = 8,
    VIDEO_COLOR_LIGHT_BLUE = 9,
    VIDEO_COLOR_LIGHT_GREEN = 10,
    VIDEO_COLOR_LIGHT_CYAN = 11,
    VIDEO_COLOR_LIGHT_RED = 12,
    VIDEO_COLOR_LIGHT_MAGENTA = 13,
    VIDEO_COLOR_LIGHT_BROWN = 14,
    VIDEO_COLOR_WHITE = 15
};

void video_init(void);
void video_clear_screen(void);
void video_put_char(char c);
void video_erase_char(void);
void video_draw_cursor(int visible);
void video_print_string(const char *str);
void video_redraw_line(int line_start, const char *buffer, int length, int cursor_pos);
int video_get_cursor_pos(void);
void video_set_cursor_pos(int pos);
unsigned char video_get_color(void);
void video_set_color(unsigned char fg, unsigned char bg);
void video_set_color_attr(unsigned char attr);

#endif // VIDEO_H
