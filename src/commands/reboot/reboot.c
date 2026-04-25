void command_reboot(void) {
    extern void video_print_string(const char *str);

    video_print_string("Rebooting Goonix...!\n");

    __asm__ volatile (
        "movl $1, %%eax\n"   /* sys_exit */
        "xorl %%ebx, %%ebx\n" /* exit code = 0 */
        "int $0x80\n"
        :
        :
        : "eax", "ebx"
    );

    __builtin_unreachable();
}