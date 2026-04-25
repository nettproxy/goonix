#include "hello.hpp"
#include "../../lib/video.hpp"

void command_hello(const char *args) {
    (void)args;
    unsigned char old = video_get_color();
    video_set_color(VIDEO_COLOR_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("Hello :3\n");
    video_set_color_attr(old);
}
