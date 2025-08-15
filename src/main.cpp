
#include <Arduino.h>
// #include <FastLED.h>
#include "burner_power.h"
#include "ina229.h"

// Brain sleep after 3 hours
// TODO: do this only when voltage is < 90%
static const int kbrainSleepSeconds = 120; // 3600 * 3;

// #define NEOPIXEL_POWER 21
// #define PIN_NEOPIXEL 33

static constexpr const char *TAG = "main";

bms_can can; // Canbus
ina229 ina;

// CRGB leds[1];

// GFXcanvas16 canvas(240, 135);

long boot_time;

void setup()
{
    boot_time = millis();
    delay(1000);
    Serial.begin(115200);
    delay(1000);
    Serial.println("starting...");
    delay(1000);
    powerpcb_init();
    delay(1000);

    // FastLED.addLeds<WS2812B, PIN_NEOPIXEL, GRB>(leds, 1);

    // pinMode(NEOPIXEL_POWER, OUTPUT);
    // digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);

    // leds[0] = CRGB::Red;
    // FastLED.show();

    Serial.println("starting canbus...");
    // can.begin(gpio_num_t::GPIO_NUM_39, gpio_num_t::GPIO_NUM_38);
    can.begin(CANBUS_RX, CANBUS_TX);

    Serial.println("starting ina...");
    ina.begin();

    ina.setShunt(0.001f);
    ina.setShuntCal(1.0f);

    BLog_d(TAG, "ina229 manf-id = %x", ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 devid = %x", ina.ReadDeviceID());
    BLog_d(TAG, "ina229 voltage = %f", ina.vBus());
    BLog_d(TAG, "ina229 current = %f", ina.current());
    BLog_d(TAG, "ina229 temperature = %f", ina.dieTemp());
}

// Power state machine
enum PowerState
{
    POWER_VESC_ACTIVE,
    POWER_VESC_OFF,
    POWER_SLEEPING
};

PowerState current_power_state = POWER_VESC_OFF;

boolean current_leds_hold_state = false;
boolean current_amp_hold_state = false;
boolean leds_hold_state;
boolean amp_hold_state;
boolean led_status = false;

boolean toggle_led()
{
    if (led_status == true)
    {
        led_status = false;
    }
    else
    {
        led_status == true;
    }
    pcb_led_set(led_status);
    return led_status;
}

uint32_t last_active_time = 0;
int iter = 0;
uint32_t fud_pressed = 0;
long up_time;

void loop()
{

    up_time = millis() - boot_time;

    delay(10);
    iter++;

    // FUD button with debounce
    if (get_fud_button())
    {
        fud_pressed++;
        // BLog_i(TAG, "fud %d", fud_pressed);
        // Also turn on brain for short press
        last_active_time = millis();
    }
    else
    {
        if (fud_pressed > 0)
        {
            fud_pressed--;
        }
    }

    // FUD won't work unless the controller has been up for 30 seconds.
    // Continuing to hold it down will turn everything on.
    if ((up_time > 30000) && (fud_pressed > 3))
    {
        BLog_i(TAG, "fud_pressed");
        fud_pressed = 0;
        set_brain_reg(false);
        delay(2000);
        set_brain_reg(false);
        delay(2000);
        esp_restart();
    }
    else if (fud_pressed > 3)
    {
        // backdoor to turn on
        BLog_i(TAG, "fud backdoor enabled; power on and leave on");
        set_auto(false);
        set_amp(true);
        set_led_master(true);
        delay(10);
        set_led_reg(true);
        set_brain_reg(true);
        delay(10);
    }

    if (get_auto())
    {
        // leds[0] = CRGB::Blue;
        // FastLED.show();
        // delay(100);
        // leds[0] = CRGB::Black;
        // FastLED.show();
        // delay(100);

        float voltage = ina.vBus();
        float current = ina.current();
        float temp = ina.dieTemp();
        int vescActive = can.vescActive() ? 1 : 0;
        int rpm = can.vescRpm();
        float vshunt = ina.vShunt() * 1000;

        if (iter % 1000 == 0)
        {
            BLog_d(TAG, "vesc = %d, rpm = %d, ina229 vbus = %f, vshunt = %f mv, current = %f", vescActive, rpm, voltage, vshunt, current);
        }

        // delay(100);
        // leds[0] = CRGB::Green;
        // FastLED.show();
        // delay(100);
        // leds[0] = CRGB::Black;
        // FastLED.show();

        // Determine new power state based on VESC activity and time
        PowerState new_power_state;

        if (can.vescActive())
        {
            last_active_time = millis();
            new_power_state = POWER_VESC_ACTIVE;
        }
        else if ((millis() - last_active_time) < (kbrainSleepSeconds * 1000))
        {
            new_power_state = POWER_VESC_OFF;
        }
        else
        {
            new_power_state = POWER_SLEEPING;
        }

        // hold logic
        // hold command -> temporary hold for x minutes
        // if hold goes from inactive to active and ! POWER_VESC_ACTIVE then turn leds on
        // if hold goes from active to inactive and ! POWER_VESC_ACTIVE then turn leds off
        // therefore, if going from POWER_VESC_ACTIVE to POWER_VESC_OFF state, check before turning lights off.
        // and if ! POWER_VESC_ACTIVE then turn on lights if hold becomes active.
        current_leds_hold_state = powerpcb_get_leds_hold();
        current_amp_hold_state = powerpcb_get_amp_hold();

        // Handle state transitions and control outputs
        if (new_power_state != current_power_state)
        {
            BLog_i(TAG, "Power state change: %d -> %d", current_power_state, new_power_state);

            switch (new_power_state)
            {
            case POWER_VESC_ACTIVE:
                // leds[0] = CRGB::Blue;
                // FastLED.show();
                set_amp(true);
                set_led_master(true);
                delay(10);
                set_led_reg(true);
                set_brain_reg(true);
                delay(10);
                break;

            case POWER_VESC_OFF:
                // leds[0] = CRGB::Green;
                // FastLED.show();
                if (!current_leds_hold_state)
                {
                    set_led_reg(false);
                    delay(10);
                    set_led_master(false);
                }
                set_brain_reg(true);
                if (!amp_hold_state)
                {
                    set_amp(false);
                }
                delay(10);
                break;

            case POWER_SLEEPING:
                // Do some low power stuff
                // FastLED.setBrightness(10);
                set_brain_reg(false);
                set_led_reg(false);
                set_amp(false);
                set_led_master(false);
                // leds[0] = CRGB::Black;
                // FastLED.show();
                break;
            }

            current_power_state = new_power_state;
        }

        // Turn on leds if leds hold  becomes true
        if ((leds_hold_state != current_leds_hold_state))
        {

            BLog_i(TAG, "leds_hold_state state change: %d -> %d", leds_hold_state, current_leds_hold_state);
            if (new_power_state != POWER_VESC_ACTIVE)
            {
                if (current_leds_hold_state == true)

                {
                    set_led_master(true);
                    delay(10);
                    set_led_reg(true);
                    delay(10);
                }

                else
                {
                    set_led_reg(false);
                    delay(10);
                    set_led_master(false);
                }
            }
        }
        leds_hold_state = current_leds_hold_state;

        // Turn on amp if leds hold  becomes true
        if ((amp_hold_state != current_amp_hold_state) && (current_amp_hold_state == true))
        {
            BLog_i(TAG, "amp_hold_state state change: %d -> %d", amp_hold_state, current_amp_hold_state);
            if (new_power_state != POWER_VESC_ACTIVE)
            {
                set_amp(true);
            }
        }
        amp_hold_state = current_amp_hold_state;

        // Handle headlight based on RPM (always checked as this is time-sensitive)
        if (can.vescRpm() > 0)
        {
            set_headlight(true);
        }
        else
        {
            set_headlight(false);
        }

        // Handle LED blinking based on current state
        if (current_power_state == POWER_VESC_ACTIVE)
        {
            if (iter % 10 == 0)
            {
                toggle_led();
            }
        }
        else if (current_power_state == POWER_VESC_OFF)
        {
            if (iter % 100 == 0)
            {
                toggle_led();
            }
        }

        // Enter sleep mode if in sleeping state
        if (current_power_state == POWER_SLEEPING)
        {
            esp_sleep_enable_timer_wakeup(1000000); // 1 sec
            esp_light_sleep_start();
            // leds[0] = CRGB::Red;
            // FastLED.show();
            // esp_sleep_enable_timer_wakeup(10000); // .1 sec
            // esp_light_sleep_start();
            // leds[0] = CRGB::Black;
            // FastLED.show();
        }
    }
}
