void command_meminfo(void) {
    extern void video_print_string(const char *str);
    extern void video_print_number(unsigned int num);
    extern unsigned char video_get_color(void);
    extern void video_set_color(unsigned char fg, unsigned char bg);
    extern void video_set_color_attr(unsigned char attr);
    extern unsigned long long total_memory;
    extern unsigned long long free_memory;

    unsigned long long reserved_memory = 0;
    if (total_memory >= free_memory) {
        reserved_memory = total_memory - free_memory;
    }

    unsigned int total_mb = (unsigned int)(total_memory >> 20);
    unsigned int free_mb = (unsigned int)(free_memory >> 20);
    unsigned int reserved_mb = (unsigned int)(reserved_memory >> 20);

    unsigned int used_mb = 0;
    extern char kernel_start;
    extern char kernel_end;
    unsigned int kernel_bytes = (unsigned int)((unsigned long)&kernel_end - (unsigned long)&kernel_start);
    used_mb = kernel_bytes >> 20;

    unsigned char old_color = video_get_color();

    video_set_color(2, 0);
    video_print_string("Total Memory: ");
    if (total_mb != 0) {
        video_print_number(total_mb);
        video_print_string("MB\n");
    } else {
        unsigned int total_kb = (unsigned int)(total_memory >> 10);
        video_print_number(total_kb);
        video_print_string("KB\n");
    }

    video_set_color(4, 0);
    video_print_string("Used Memory: ");
    if (used_mb != 0) {
        video_print_number(used_mb);
        video_print_string("MB\n");
    } else {
        unsigned int used_kb = (unsigned int)(kernel_bytes >> 10);
        video_print_number(used_kb);
        video_print_string("KB\n");
    }

    video_set_color(1, 0);
    video_print_string("Reserved Memory: ");
    if (reserved_mb != 0) {
        video_print_number(reserved_mb);
        video_print_string("MB\n");
    } else {
        unsigned int reserved_kb = (unsigned int)(reserved_memory >> 10);
        video_print_number(reserved_kb);
        video_print_string("KB\n");
    }

    video_set_color_attr(old_color);
}
