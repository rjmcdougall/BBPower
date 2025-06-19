#include "burner_power.h"

static constexpr const char *TAG = "terminal";

void terminal_process_string(char *str)
{
    BLog_i(TAG, "Terminal Command: %s\n", str);

    const int kMaxArgs = 64;
    char *argv[kMaxArgs];
    int argc = 0;

    // Use a for loop to handle initialization, condition, and the next token.
    // Also, use a more descriptive variable name like "token".
    for (char *token = strtok(str, " "); token != NULL && argc < kMaxArgs; token = strtok(NULL, " "))
    {
        argv[argc++] = token;
    }

    bms_can::commands_printf("Hello from terminal!\n");

    if (argc == 0)
    {
        bms_can::commands_printf("No command received\n");
        return;
    }

    if (strcmp(argv[0], "stats") == 0)
    {
        bms_can::commands_printf("stats\n");
        bms_can::commands_printf("vesc = %d, rpm = %f, ina229 vbus = %f, vshunt = %f mv, current = %f", can.vescActive(), can.vescRpm(), ina.vBus(), ina.vShunt() * 1000, ina.current());
    }
}
