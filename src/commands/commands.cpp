#include "../lib/video.hpp"
#include "../lib/utils/utils.hpp"
#include "command.hpp"
#include <stdint.h>

#include "clear/clear.hpp"
#include "hello/hello.hpp"
#include "random/random.hpp"
#include "reboot/reboot.hpp"
#include "meminfo/meminfo.hpp"
#include "halt/halt.hpp"
#include "clrtest/clrtest.hpp"
#include "help/help.hpp"
#include "echo/echo.hpp"
#include "info/info.hpp"
#include "cpustate/cpustate.hpp"

using namespace lib::utils;
using namespace lib::video;

Command commands[] = {
    {"help", command_help, "Display information about built-in commands"},
    {"clear", command_clear, "Clear the terminal screen"},
    {"hello", command_hello, "Display a greeting"},
    {"reboot", command_reboot, "Restart the system"},
    {"halt", command_halt, "Halt the CPU"},
    {"meminfo", command_meminfo, "Display memory usage information"},
    {"info", command_info, "Display CPU and system information"},
    {"clrtest", command_clrtest, "Print color test lines"},
    {"echo", command_echo, "Print a string"},
    {"random", command_random, "Test"},
    {"cpustate", command_cpustate, "Display CPU registers and state"},
    {0, 0, 0}
};
