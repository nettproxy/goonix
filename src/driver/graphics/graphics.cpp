#include "graphics.hpp"
#include <stdint.h>

namespace driver {
namespace graphics {

const char *GraphicsDriver::gpu_name = "Unknown GPU";
const char *GraphicsDriver::monitor_name = "Generic VGA Monitor";

static char monitor_name_buf[14];

static inline uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func,
                                       uint8_t offset) {
  uint32_t address = (uint32_t)((uint32_t)bus << 16) | ((uint32_t)slot << 11) |
                     ((uint32_t)func << 8) | (offset & 0xfc) |
                     ((uint32_t)0x80000000);
  asm volatile("outl %0, %%dx" : : "a"(address), "d"((uint16_t)0xCF8));
  uint32_t tmp;
  asm volatile("inl %%dx, %0" : "=a"(tmp) : "d"((uint16_t)0xCFC));
  return tmp;
}

void GraphicsDriver::detect_gpu() {
  for (uint16_t bus = 0; bus < 256; bus++) {
    for (uint8_t slot = 0; slot < 32; slot++) {
      uint32_t id = pci_read_config(bus, slot, 0, 0);
      if (id != 0xFFFFFFFF) {
        uint32_t class_rev = pci_read_config(bus, slot, 0, 0x08);
        uint8_t class_code = (class_rev >> 24) & 0xFF;
        if (class_code == 0x03) {
          uint16_t vendor = id & 0xFFFF;
          uint16_t device = (id >> 16) & 0xFFFF;

          if (vendor == 0x1234 && device == 0x1111)
            gpu_name = "Bochs/QEMU VBE Adapter";
          else if (vendor == 0x80ee && device == 0xbeef)
            gpu_name = "VirtualBox Graphics Adapter";
          else if (vendor == 0x10de)
            /* https://the-sz.com/products/usbid/index.php?v={vendorID}&p=&n=
               (I used this as offset with vendor to get name) */
            gpu_name = "NVIDIA Graphics Card";
          else if (vendor == 0x1002)
            gpu_name = "AMD/ATI Graphics Card";
          else if (vendor == 0x15AD)
            gpu_name = "VMWare Graphics Card";
          else if (vendor == 0x8086)
            gpu_name = "Intel Graphics Controller";
          else
            gpu_name = "Standard VGA Controller";
          return;
        }
      }
    }
  }
}

bool GraphicsDriver::is_edid(const unsigned char *data) {
  const uint8_t header[] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
  for (int i = 0; i < 8; i++) {
    if (data[i] != header[i])
      return false;
  }
  return true;
}

void GraphicsDriver::parse_edid_name(const unsigned char *edid) {
  const int offsets[] = {54, 72, 90, 108};
  for (int i = 0; i < 4; i++) {
    const uint8_t *d = edid + offsets[i];
    if (d[0] == 0x00 && d[1] == 0x00 && d[2] == 0x00 && d[3] == 0xFC) {
      int k = 0;
      for (int j = 5; j < 18 && k < 13; j++) {
        if (d[j] == 0x0A)
          break;
        monitor_name_buf[k++] = (char)d[j];
      }
      monitor_name_buf[k] = '\0';

      while (k > 0 && monitor_name_buf[k - 1] == ' ') {
        monitor_name_buf[--k] = '\0';
      }

      if (k > 0) {
        monitor_name = monitor_name_buf;
        return;
      }
    }
  }
}

void GraphicsDriver::detect_monitor() {
  for (uint32_t addr = 0xC0000; addr < 0x100000; addr += 16) {
    const unsigned char *ptr = (const unsigned char *)addr;
    if (is_edid(ptr)) {
      parse_edid_name(ptr);
      return;
    }
  }

  for (uint32_t addr = 0x90000; addr < 0xA0000; addr += 16) {
    const unsigned char *ptr = (const unsigned char *)addr;
    if (is_edid(ptr)) {
      parse_edid_name(ptr);
      return;
    }
  }
}

void GraphicsDriver::init() {
  detect_gpu();
  detect_monitor();
}

const char *GraphicsDriver::get_gpu_name() { return gpu_name; }

const char *GraphicsDriver::get_monitor_name() { return monitor_name; }

} // namespace graphics
} // namespace driver
