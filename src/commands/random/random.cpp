#include "random.hpp"
#include "../../lib/video.hpp"

static unsigned int seed = 123456;

static unsigned int simple_rand() {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

static void itoa(int value, char* str) {
    char buffer[16];
    int i = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = 0; j < i; j++) {
        str[j] = buffer[i - j - 1];
    }
    str[i] = '\0';
}

void command_random(const char *args) {
    char buf[16];
    int num = simple_rand() % 100 + 1;
    itoa(num, buf);
    video_print_string(buf);
    video_print_string("\n");
} // Ts would be so easier if we could use rand() bro..