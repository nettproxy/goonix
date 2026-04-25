#include "utils.hpp"

namespace lib {
namespace utils {

int string_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

int string_length(const char *s) {
    int n = 0;
    while (*s++) n++;
    return n;
}

int min3_int(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

int levenshtein_distance(const char *a, const char *b) {
    int la = string_length(a);
    int lb = string_length(b);
    if (la > 63) la = 63;
    if (lb > 63) lb = 63;

    int prev[64];
    int cur[64];

    for (int j = 0; j <= lb; j++) {
        prev[j] = j;
    }

    for (int i = 1; i <= la; i++) {
        cur[0] = i;
        for (int j = 1; j <= lb; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            cur[j] = min3_int(
                prev[j] + 1,
                cur[j - 1] + 1,
                prev[j - 1] + cost
            );
        }
        for (int j = 0; j <= lb; j++) {
            prev[j] = cur[j];
        }
    }

    return prev[lb];
} // Yes, I did fucking use AI for this

void sanitize_ascii(char *s) {
    while (*s) {
        unsigned char ch = (unsigned char)*s;
        if (ch < 32 || ch > 126) {
            *s = '?';
        }
        s++;
    }
}

} // namespace utils
} // namespace lib
