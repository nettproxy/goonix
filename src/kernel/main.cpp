#include "../shell/shell.cpp"

extern "C" void kernel_main(unsigned int magic, unsigned int addr) {
    multiboot_info_t *mb = (multiboot_info_t *)addr;

    parse_memory(mb);

    shell_init(addr);
    shell_run();
}
