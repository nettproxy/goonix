#ifndef VIDEO_HPP
#define VIDEO_HPP

namespace lib {
namespace video {

enum Color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15
};

void init();
void clear_screen();
void put_char(char c);
void erase_char();
void draw_cursor(int visible);
void print_string(const char *str);
void redraw_line(int line_start, const char *buffer, int length, int cursor_pos);
int get_cursor_pos();
void set_cursor_pos(int pos);
unsigned char get_color();
void set_color(unsigned char fg, unsigned char bg);
void set_color_attr(unsigned char attr);

} // namespace video
} // namespace lib

// C bindings for compatibility
#define VIDEO_COLOR_BLACK lib::video::COLOR_BLACK
#define VIDEO_COLOR_BLUE lib::video::COLOR_BLUE
#define VIDEO_COLOR_GREEN lib::video::COLOR_GREEN
#define VIDEO_COLOR_CYAN lib::video::COLOR_CYAN
#define VIDEO_COLOR_RED lib::video::COLOR_RED
#define VIDEO_COLOR_MAGENTA lib::video::COLOR_MAGENTA
#define VIDEO_COLOR_BROWN lib::video::COLOR_BROWN
#define VIDEO_COLOR_LIGHT_GREY lib::video::COLOR_LIGHT_GREY
#define VIDEO_COLOR_DARK_GREY lib::video::COLOR_DARK_GREY
#define VIDEO_COLOR_LIGHT_BLUE lib::video::COLOR_LIGHT_BLUE
#define VIDEO_COLOR_LIGHT_GREEN lib::video::COLOR_LIGHT_GREEN
#define VIDEO_COLOR_LIGHT_CYAN lib::video::COLOR_LIGHT_CYAN
#define VIDEO_COLOR_LIGHT_RED lib::video::COLOR_LIGHT_RED
#define VIDEO_COLOR_LIGHT_MAGENTA lib::video::COLOR_LIGHT_MAGENTA
#define VIDEO_COLOR_LIGHT_BROWN lib::video::COLOR_LIGHT_BROWN
#define VIDEO_COLOR_WHITE lib::video::COLOR_WHITE

extern "C" {
    inline void video_init() { lib::video::init(); }
    inline void video_clear_screen() { lib::video::clear_screen(); }
    inline void video_put_char(char c) { lib::video::put_char(c); }
    inline void video_erase_char() { lib::video::erase_char(); }
    inline void video_draw_cursor(int visible) { lib::video::draw_cursor(visible); }
    inline void video_print_string(const char *str) { lib::video::print_string(str); }
    inline void video_redraw_line(int line_start, const char *buffer, int length, int cursor_pos) { lib::video::redraw_line(line_start, buffer, length, cursor_pos); }
    inline int video_get_cursor_pos() { return lib::video::get_cursor_pos(); }
    inline void video_set_cursor_pos(int pos) { lib::video::set_cursor_pos(pos); }
    inline unsigned char video_get_color() { return lib::video::get_color(); }
    inline void video_set_color(unsigned char fg, unsigned char bg) { lib::video::set_color(fg, bg); }
    inline void video_set_color_attr(unsigned char attr) { lib::video::set_color_attr(attr); }
}

#endif // VIDEO_HPP
