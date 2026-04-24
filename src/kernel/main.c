#include "../shell/shell.c"

void kernel_main(void) {
    shell_init();
    shell_run();
}
