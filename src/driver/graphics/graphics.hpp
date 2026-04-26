#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

namespace driver {
namespace graphics {

class GraphicsDriver {
public:
  static void init();
  static const char *get_gpu_name();
  static const char *get_monitor_name();

private:
  static const char *gpu_name;
  static const char *monitor_name;

  static void detect_gpu();
  static void detect_monitor();
  static bool is_edid(const unsigned char *data);
  static void parse_edid_name(const unsigned char *edid);
};

} // namespace graphics
} // namespace driver

#endif
