#include "burner_power.h"


static constexpr const char *TAG = "power_pcb";

static boolean headlight_state = false;
static boolean brain_state = false;
static boolean led_master_state = false;
static boolean led_state = false;
static boolean amp_state = false;
static boolean auto_state = true;

void powerpcb_init()
{

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(HEADLIGHT_ON, OUTPUT);
    digitalWrite(HEADLIGHT_ON, LOW);

    pinMode(LEG_REG_RELAY_OFF, OUTPUT); // LED REG RELAY HIGH=OFF
    digitalWrite(LEG_REG_RELAY_OFF, HIGH);

    pinMode(AMP_RELAY_OFF, OUTPUT); // AMP RELAY HIGH=OFF
    digitalWrite(AMP_RELAY_OFF, HIGH);

    pinMode(LEG_REG_DISABLE, OUTPUT); // LED_REG ENABLE HIGH=OFF
    digitalWrite(LEG_REG_DISABLE, HIGH);

    pinMode(REG_5V_DISABLE, OUTPUT); // 5V REG ENALE HIGH=OFF
    digitalWrite(REG_5V_DISABLE, LOW);

    pinMode(FUD_BUTTON, INPUT_PULLUP);

}

void set_headlight(boolean state)
{
    headlight_state = state;
    digitalWrite(HEADLIGHT_ON, state ? HIGH : LOW);
}

void set_brain_reg(boolean state)
{
    brain_state = state;
    digitalWrite(REG_5V_DISABLE, state ? LOW : HIGH);
}

void set_led_reg(boolean state)
{
    led_state = state;
    digitalWrite(LEG_REG_DISABLE, state ? LOW : HIGH);
}

void set_led_master(boolean state)
{
    led_master_state = state;
    digitalWrite(LEG_REG_RELAY_OFF, state ? LOW : HIGH);
}


void set_auto(boolean state)
{
    auto_state = state;
}

void set_amp(boolean state)
{
    amp_state = state;
    digitalWrite(AMP_RELAY_OFF, state ? LOW : HIGH);
}

boolean get_headlight()
{
    return headlight_state;
}



boolean get_brain_reg()
{
    return brain_state;
}

boolean get_led_reg()
{
    return led_state;
}

boolean get_led_master()
{
    return led_master_state;
}


boolean get_auto()
{
    return auto_state;
}

boolean get_amp()
{
    return amp_state;
}

boolean get_fud_button() {
    //int fud = digitalRead(FUD_BUTTON);
    int fud = analogRead(FUD_BUTTON);
    //BLog_d(TAG, "fud = %s", digitalRead(FUD_BUTTON) == HIGH ? "HIGH" : "LOW");
    BLog_d(TAG, "fud = %d", fud);
    //BLog_d(TAG, "fud = %d", analogRead(FUD_BUTTON));
    //return (digitalRead(FUD_BUTTON) == LOW);
    return false;
}
