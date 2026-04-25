#include "halt.hpp"

void command_halt(const char *args) {
    (void)args;
    __asm__ volatile("hlt");
}
