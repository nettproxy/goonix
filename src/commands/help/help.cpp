#include "help.hpp"
#include "../../lib/video.hpp"
#include "../command.hpp"

void command_help(const char *args) {
    (void)args;
    unsigned char old = video_get_color();

    video_set_color(VIDEO_COLOR_LIGHT_BLUE, VIDEO_COLOR_BLACK);
    video_print_string("Commands:\n");

    for (int i = 0; commands[i].name; i++) {
        video_set_color(VIDEO_COLOR_CYAN, VIDEO_COLOR_BLACK);
        video_print_string("  ");
        video_print_string(commands[i].name);
        video_set_color(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK);
        video_print_string(" - ");
        video_set_color(VIDEO_COLOR_LIGHT_GREY, VIDEO_COLOR_BLACK);
        video_print_string(commands[i].desc);
        video_print_string("\n");
    }

    video_set_color_attr(old);
}
