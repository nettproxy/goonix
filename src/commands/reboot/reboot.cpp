#include "reboot.hpp"
#include "../../lib/video.hpp"

void command_reboot(const char *args) {
    (void)args;
    video_print_string("Rebooting Goonix...!\n");

    __asm__ volatile (
        "movl $1, %%eax\n"
        "xorl %%ebx, %%ebx\n"
        "int $0x80\n"
        :
        :
        : "eax", "ebx"
    );

    __builtin_unreachable();
}
