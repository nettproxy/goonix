#include "info.hpp"
#include "../../lib/utils/utils.hpp"
#include "../../lib/video.hpp"
#include "../../driver/graphics/graphics.hpp"
#include <stdint.h>

extern void video_print_number(unsigned int num);

static int cpuid_supported() {
  uint32_t before;
  uint32_t after;
  __asm__ volatile("pushfl\n\t"
                   "popl %0\n\t"
                   : "=r"(before)
                   :
                   : "cc");

  uint32_t toggled = before ^ (1u << 21);
  __asm__ volatile("pushl %0\n\t"
                   "popfl\n\t"
                   :
                   : "r"(toggled)
                   : "cc");

  __asm__ volatile("pushfl\n\t"
                   "popl %0\n\t"
                   : "=r"(after)
                   :
                   : "cc");

  return ((before ^ after) & (1u << 21)) != 0;
}

static void cpuid(uint32_t leaf, uint32_t subleaf, uint32_t *a, uint32_t *b,
                  uint32_t *c, uint32_t *d) {
  uint32_t eax_inout = leaf;
  uint32_t ecx_inout = subleaf;
  uint32_t ebx_out;
  uint32_t edx_out;
  __asm__ volatile("pushl %%ebx\n\t"
                   "cpuid\n\t"
                   "movl %%ebx, %1\n\t"
                   "popl %%ebx\n\t"
                   : "+a"(eax_inout), "=m"(ebx_out), "+c"(ecx_inout),
                     "=d"(edx_out)
                   :
                   : "cc");
  *a = eax_inout;
  *b = ebx_out;
  *c = ecx_inout;
  *d = edx_out;
}

static void write_u32_to_bytes(uint32_t v, char *out) {
  out[0] = (char)(v & 0xFF);
  out[1] = (char)((v >> 8) & 0xFF);
  out[2] = (char)((v >> 16) & 0xFF);
  out[3] = (char)((v >> 24) & 0xFF);
}

static int contains_non_space(const char *s) {
  while (*s) {
    if (*s != ' ')
      return 1;
    s++;
  }
  return 0;
}

static void print_feature(const char *name) {
  video_print_string(name);
  video_print_string(" ");
}

void command_info(const char *args) {
  (void)args;
  unsigned char old = video_get_color();
  video_set_color(9, 0);
  video_set_color_attr(old);

  if (!cpuid_supported()) {
    video_set_color(3, 0);
    video_print_string("CPU Vendor: ");
    video_set_color(12, 0);
    video_print_string("unavailable\n");
    video_set_color(3, 0);
    video_print_string("CPU Brand: ");
    video_set_color(12, 0);
    video_print_string("unavailable\n");
    video_set_color_attr(old);
    return;
  }

  uint32_t a, b, c, d;
  cpuid(0, 0, &a, &b, &c, &d);
  uint32_t max_basic = a;

  char vendor[13];
  write_u32_to_bytes(b, &vendor[0]);
  write_u32_to_bytes(d, &vendor[4]);
  write_u32_to_bytes(c, &vendor[8]);
  vendor[12] = 0;
  lib::utils::sanitize_ascii(vendor);

  video_set_color(3, 0);
  video_print_string("CPU Vendor: ");
  video_set_color(10, 0);
  video_print_string(vendor);
  video_set_color_attr(old);
  video_print_string("\n");

  char brand[49];
  brand[0] = 0;
  cpuid(0x80000000u, 0, &a, &b, &c, &d);
  uint32_t max_ext = a;
  if (max_ext >= 0x80000004u) {
    cpuid(0x80000002u, 0, &a, &b, &c, &d);
    write_u32_to_bytes(a, &brand[0]);
    write_u32_to_bytes(b, &brand[4]);
    write_u32_to_bytes(c, &brand[8]);
    write_u32_to_bytes(d, &brand[12]);

    cpuid(0x80000003u, 0, &a, &b, &c, &d);
    write_u32_to_bytes(a, &brand[16]);
    write_u32_to_bytes(b, &brand[20]);
    write_u32_to_bytes(c, &brand[24]);
    write_u32_to_bytes(d, &brand[28]);

    cpuid(0x80000004u, 0, &a, &b, &c, &d);
    write_u32_to_bytes(a, &brand[32]);
    write_u32_to_bytes(b, &brand[36]);
    write_u32_to_bytes(c, &brand[40]);
    write_u32_to_bytes(d, &brand[44]);

    brand[48] = 0;
  }
  lib::utils::sanitize_ascii(brand);

  video_set_color(3, 0);
  video_print_string("CPU Brand: ");
  if (contains_non_space(brand)) {
    video_set_color(10, 0);
    video_print_string(brand);
    video_set_color_attr(old);
  } else {
    video_set_color(12, 0);
    video_print_string("unavailable");
    video_set_color_attr(old);
  }
  video_print_string("\n");

  if (max_basic >= 1) {
    cpuid(1, 0, &a, &b, &c, &d);

    uint32_t stepping = a & 0xF;
    uint32_t model = (a >> 4) & 0xF;
    uint32_t family = (a >> 8) & 0xF;
    uint32_t ext_model = (a >> 16) & 0xF;
    uint32_t ext_family = (a >> 20) & 0xFF;

    uint32_t display_family = family;
    uint32_t display_model = model;
    if (family == 0xF) {
      display_family = family + ext_family;
    }
    if (family == 0x6 || family == 0xF) {
      display_model = model + (ext_model << 4);
    }

    video_set_color(3, 0);
    video_print_string("CPU Family: ");
    video_set_color(10, 0);
    video_print_number(display_family);
    video_set_color_attr(old);
    video_print_string("\n");

    video_set_color(3, 0);
    video_print_string("CPU Features: ");
    video_set_color(10, 0);

    if (d & (1u << 0))
      print_feature("FPU");
    if (d & (1u << 9))
      print_feature("APIC");
    if (d & (1u << 23))
      print_feature("MMX");
    if (d & (1u << 25))
      print_feature("SSE");
    if (d & (1u << 26))
      print_feature("SSE2");

    if (c & (1u << 0))
      print_feature("SSE3");
    if (c & (1u << 9))
      print_feature("SSSE3");
    if (c & (1u << 19))
      print_feature("SSE4.1");
    if (c & (1u << 20))
      print_feature("SSE4.2");
    if (c & (1u << 28))
      print_feature("AVX");

    video_set_color_attr(old);
    video_print_string("\n");
  }

  video_set_color(3, 0);
  video_print_string("Arch: ");
  video_set_color(10, 0);
  video_print_string("x86 (i386)\n");
  video_set_color(3, 0);
  video_print_string("Boot: ");
  video_set_color(10, 0);
  video_print_string("GRUB Multiboot v1\n");
  video_set_color(3, 0);
  video_print_string("Resolution: ");
  video_set_color(10, 0);
  video_print_number(video_get_screen_width());
  video_print_string("x");
  video_print_number(video_get_screen_height());
  video_print_string(" (VGA text mode)\n");
  video_set_color(3, 0);
  video_print_string("GPU: ");
  video_set_color(10, 0);
  video_print_string(driver::graphics::GraphicsDriver::get_gpu_name());
  video_print_string("\n");
  video_set_color(3, 0);
  video_print_string("Monitor: ");
  video_set_color(10, 0);
  video_print_string(driver::graphics::GraphicsDriver::get_monitor_name());
  video_print_string("\n");
  video_set_color_attr(old);
}
