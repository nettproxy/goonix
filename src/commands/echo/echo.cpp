#include "echo.hpp"
#include "../../lib/video.hpp"

void command_echo(const char *str) {
    if (str) {
        video_print_string(str);
        video_print_string("\n");
    }
}
