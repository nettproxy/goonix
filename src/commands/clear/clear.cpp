#include "clear.hpp"
#include "../../lib/video.hpp"

void command_clear(const char *args) {
    (void)args;
    video_clear_screen();
}
