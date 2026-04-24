void command_hello(void) {
    extern void video_print_string(const char *str);
    video_print_string("Hello   !\n");
}
