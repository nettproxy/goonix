#ifndef UTILS_HPP
#define UTILS_HPP

namespace lib {
namespace utils {

int string_equals(const char *a, const char *b);
int string_length(const char *s);
int min3_int(int a, int b, int c);
int levenshtein_distance(const char *a, const char *b);
void sanitize_ascii(char *s);

} // namespace utils
} // namespace lib

#endif
