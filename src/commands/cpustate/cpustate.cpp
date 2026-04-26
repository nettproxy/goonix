#include "cpustate.hpp"
#include "../../lib/utils/utils.hpp"
#include "../../lib/video.hpp"
#include <stdint.h>

extern void video_print_number(unsigned int num);

static void print_reg(const char *name, uint32_t val, unsigned char color_name,
                      unsigned char color_val) {
  char hex[11];
  lib::utils::int_to_hex(val, hex);

  video_set_color(color_name, 0);
  video_print_string(name);
  video_print_string(": ");
  video_set_color(color_val, 0);
  video_print_string(hex);
  video_print_string("  ");
}

void command_cpustate(const char *args) {
  (void)args;
  uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags;
  uint32_t cr0, cr2, cr3, cr4;
  uint16_t cs, ds, ss, es, fs, gs;

  asm volatile("mov %%eax, %0" : "=r"(eax));
  asm volatile("mov %%ebx, %0" : "=r"(ebx));
  asm volatile("mov %%ecx, %0" : "=r"(ecx));
  asm volatile("mov %%edx, %0" : "=r"(edx));
  asm volatile("mov %%esi, %0" : "=r"(esi));
  asm volatile("mov %%edi, %0" : "=r"(edi));
  asm volatile("mov %%esp, %0" : "=r"(esp));
  asm volatile("mov %%ebp, %0" : "=r"(ebp));

  asm volatile("call 1f\n1: pop %0" : "=r"(eip));

  asm volatile("pushfl\npop %0" : "=r"(eflags));

  asm volatile("mov %%cs, %0" : "=r"(cs));
  asm volatile("mov %%ds, %0" : "=r"(ds)); // fuck asm
  asm volatile("mov %%ss, %0" : "=r"(ss));
  asm volatile("mov %%es, %0" : "=r"(es));
  asm volatile("mov %%fs, %0" : "=r"(fs));
  asm volatile("mov %%gs, %0" : "=r"(gs));

  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  asm volatile("mov %%cr2, %0" : "=r"(cr2));
  asm volatile("mov %%cr3, %0" : "=r"(cr3));
  asm volatile("mov %%cr4, %0" : "=r"(cr4));

  unsigned char old = video_get_color();

  video_set_color(11, 0);
  video_set_color(8, 0);
  video_print_string("--------------------------------------------------\n");

  print_reg("EAX", eax, 3, 10);
  print_reg("EBX", ebx, 3, 10);
  print_reg("ECX", ecx, 3, 10);
  video_print_string("\n");
  print_reg("EDX", edx, 3, 10);
  print_reg("ESI", esi, 3, 10);
  print_reg("EDI", edi, 3, 10);
  video_print_string("\n\n");

  print_reg("ESP", esp, 3, 14);
  print_reg("EBP", ebp, 3, 14);
  print_reg("EIP", eip, 12, 15);
  video_print_string("\n");
  print_reg("EFL", eflags, 3, 13);
  video_print_string("\n\n");

  video_set_color(11, 0);
  video_print_string(" Segments:\n ");
  print_reg("CS", cs, 7, 15);
  print_reg("DS", ds, 7, 15);
  print_reg("SS", ss, 7, 15);
  video_print_string("\n ");
  print_reg("ES", es, 7, 15);
  print_reg("FS", fs, 7, 15);
  print_reg("GS", gs, 7, 15);
  video_print_string("\n\n");

  video_set_color(11, 0);
  video_print_string(" Control:\n ");
  print_reg("CR0", cr0, 6, 7);
  print_reg("CR2", cr2, 6, 7);
  video_print_string("\n ");
  print_reg("CR3", cr3, 6, 7);
  print_reg("CR4", cr4, 6, 7);
  video_print_string("\n\n");

  video_set_color(11, 0);
  video_print_string(" instruction stream @ EIP:\n ");
  video_set_color(14, 0);
  uint8_t *ptr = (uint8_t *)eip;
  for (int i = 0; i < 8; i++) {
    uint8_t b = ptr[i];
    const char *hex_chars = "0123456789ABCDEF";
    char b_hex[3] = {hex_chars[b >> 4], hex_chars[b & 0xF], ' '};
    char b_str[4] = {b_hex[0], b_hex[1], ' ', '\0'};
    video_print_string(b_str);
  }
  video_print_string("\n");

  video_set_color(8, 0);
  video_print_string("--------------------------------------------------\n");
  video_set_color_attr(old);
}
