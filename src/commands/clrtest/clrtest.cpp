#include "clrtest.hpp"
#include "../../lib/video.hpp"

void command_clrtest(const char *args) {
    (void)args;
    unsigned char old = video_get_color();
    video_set_color(VIDEO_COLOR_GREEN, VIDEO_COLOR_BLACK);
    video_print_string("Green\n");
    video_set_color(VIDEO_COLOR_RED, VIDEO_COLOR_BLACK);
    video_print_string("Red\n");
    video_set_color(VIDEO_COLOR_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("Blue\n");
    video_set_color(VIDEO_COLOR_LIGHT_BROWN, VIDEO_COLOR_BLACK);
    video_print_string("Light Brown\n");
    video_set_color(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK);
    video_print_string("White\n");
    video_set_color(VIDEO_COLOR_BLACK, VIDEO_COLOR_WHITE);
    video_print_string("Black\n");
    video_set_color(VIDEO_COLOR_CYAN, VIDEO_COLOR_BLACK);
    video_print_string("Cyan\n");
    video_set_color(VIDEO_COLOR_MAGENTA, VIDEO_COLOR_BLACK);
    video_print_string("Magenta\n");
    video_set_color(VIDEO_COLOR_LIGHT_GREEN, VIDEO_COLOR_BLACK);
    video_print_string("Light Green\n");
    video_set_color_attr(old);
}
