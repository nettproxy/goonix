#ifndef COMMAND_HPP
#define COMMAND_HPP

typedef struct {
    const char *name;
    void (*fn)(const char *args);
    const char *desc;
} Command;

extern Command commands[];

#endif
