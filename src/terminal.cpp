#include "burner_power.h"

static constexpr const char *TAG = "terminal";

void terminal_process_string(char *str)
{
    BLog_i(TAG, "Terminal Command: %s\n", str);
    bms_can::commands_printf("Terminal: %s\n", str);


    char cmd[128];
    const int kMaxArgs = 64;
    char *argv[kMaxArgs];
    int argc = 0;

    strncpy(cmd, str, 128);

    for (char *token = strtok(str, " "); token != NULL && argc < kMaxArgs; token = strtok(NULL, " "))
    {
        argv[argc++] = token;
    }

    if (argc == 0)
    {
        bms_can::commands_printf("No command received\n");
        return;
    }

    if (strcmp(argv[0], "stats") == 0)
    {
        bms_can::commands_printf("stats\n");
        bms_can::commands_printf("vesc =      %d, rpm = %f\nina229 vbus = %f, vshunt = %f mv, current = %f", can.vescActive(), can.vescRpm(), ina.vBus(), ina.vShunt() * 1000, ina.current());
        bms_can::commands_printf("auto:       %s", get_auto() ? "ON" : "OFF");
        bms_can::commands_printf("brain:      %s", get_brain_reg() ? "ON" : "OFF");
        bms_can::commands_printf("amp:        %s", get_amp() ? "ON" : "OFF");
        bms_can::commands_printf("headlight:  %s", get_headlight() ? "ON" : "OFF");
        bms_can::commands_printf("led_master: %s", get_led_master() ? "ON" : "OFF");
        bms_can::commands_printf("led_reg:    %s", get_led_reg() ? "ON" : "OFF");

        bms_can::commands_printf("\n");
        bms_can::terminal_stats();

    }
    else if (strcmp(cmd, "b1") == 0)
    {
        set_brain_reg(true);
        bms_can::commands_printf("brain:      %s", get_brain_reg() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "b0") == 0)
    {
        set_brain_reg(false);
        bms_can::commands_printf("brain:      %s", get_brain_reg() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "h1") == 0)
    {
        set_headlight(true);
        bms_can::commands_printf("headlight:  %s", get_headlight() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "h0") == 0)
    {
        set_headlight(false);
        bms_can::commands_printf("headlight:  %s", get_headlight() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "lm1") == 0)
    {
        set_led_master(true);
        bms_can::commands_printf("led_master: %s", get_led_master() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "lm0") == 0)
    {
        set_led_master(false);
        bms_can::commands_printf("led_master: %s", get_led_master() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "l1") == 0)
    {
        set_led_reg(true);
        bms_can::commands_printf("led_reg:    %s", get_led_reg() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "l0") == 0)
    {
        set_led_reg(false);
        bms_can::commands_printf("led_reg:    %s", get_led_reg() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "amp1") == 0)
    {
        set_amp(true);
        bms_can::commands_printf("amp:        %s", get_amp() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "amp0") == 0)
    {
        set_amp(false);
        bms_can::commands_printf("amp:        %s", get_amp() ? "ON" : "OFF");
    }
    else if (strcmp(cmd, "a1") == 0)
    {
        set_auto(true);
    }
    else if (strcmp(cmd, "a0") == 0)
    {
        set_auto(false);
        bms_can::commands_printf("auto:       %s", get_auto() ? "ON" : "OFF");
    }
    else
    {
        bms_can::commands_printf("stats\n");
        bms_can::commands_printf("b1|0 brain on|off\n");
        bms_can::commands_printf("h1|0 headlight on|off\n");
        bms_can::commands_printf("lm1|0 ledmaster on|off\n");
        bms_can::commands_printf("l1|0 leds on|off\n");
        bms_can::commands_printf("amp1|0 amp on|off\n");
        bms_can::commands_printf("a1|0 auto on|off\n");
    }
}
