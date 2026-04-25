#include "../../lib/video.h"

void command_hello(void) {
    extern void video_print_string(const char *str);
    extern unsigned char video_get_color(void);
    extern void video_set_color(unsigned char fg, unsigned char bg);
    extern void video_set_color_attr(unsigned char attr);
    extern void video_print_string(const char *str);

    unsigned char old = video_get_color();
    video_set_color(VIDEO_COLOR_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("Hello :3\n");
    video_set_color_attr(old);
}
