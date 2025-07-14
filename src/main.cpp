
#include <Arduino.h>
//#include <FastLED.h>
#include "burner_power.h"
#include "ina229.h"

// Brain sleep after 3 hours
// TODO: do this only when voltage is < 90%
static const int kbrainSleepSeconds = 3600 * 3;

// #define NEOPIXEL_POWER 21
// #define PIN_NEOPIXEL 33


static constexpr const char *TAG = "main";

bms_can can; // Canbus
ina229 ina;

//CRGB leds[1];

// GFXcanvas16 canvas(240, 135);

void setup()
{
    delay(1000);
    Serial.begin(115200);
    delay(1000);
    Serial.println("starting...");
    delay(1000);
    powerpcb_init();
    delay(1000);
  
    //FastLED.addLeds<WS2812B, PIN_NEOPIXEL, GRB>(leds, 1);

    //pinMode(NEOPIXEL_POWER, OUTPUT);
    //digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);

    //leds[0] = CRGB::Red;
    //FastLED.show();

    Serial.println("starting canbus...");
    //can.begin(gpio_num_t::GPIO_NUM_39, gpio_num_t::GPIO_NUM_38);
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

uint32_t last_active_time = 0;

void loop()
{

    uint32_t fud_pressed = 0;

    delay(100);

    if (get_auto())
    {
        //leds[0] = CRGB::Blue;
        //FastLED.show();
        delay(100);
        //leds[0] = CRGB::Black;
        //FastLED.show();
        delay(100);


        float voltage = ina.vBus();
        float current = ina.current();
        float temp = ina.dieTemp();

        BLog_d(TAG, "vesc = %d, rpm = %f, ina229 vbus = %f, vshunt = %f mv, current = %f", can.vescActive(), can.vescRpm(), ina.vBus(), ina.vShunt() * 1000, ina.current());

        delay(100);
        //leds[0] = CRGB::Green;
        //FastLED.show();
        delay(100);
        //leds[0] = CRGB::Black;
        //FastLED.show();

        // FUD button with debounce
        if (get_fud_button()) {
            fud_pressed++;
        } else {
            if (fud_pressed > 0) {
                fud_pressed--;
            }
        }

        if (fud_pressed > 3) {
            BLog_i(TAG, "fud_pressed");
            fud_pressed = 0;
            set_brain_reg(false);
            delay(2000);
            set_brain_reg(false);
        }


        if (can.vescRpm() > 0)
        {
            set_headlight(true);
        }
        else
        {
            set_headlight(false);
        }

        if (can.vescActive())
        {
            last_active_time = millis();
            //leds[0] = CRGB::Blue;
            //FastLED.show();
            set_amp(true);
            set_led_master(true);
            delay(10);
            set_led_reg(true);
            //leds[0] = CRGB::Black;
            //FastLED.show();
            delay(100);
        }
        else
        {
            //eds[0] = CRGB::Green;
            //FastLED.show();
            set_led_reg(false);
            delay(10);
            set_amp(false);
            set_led_reg(false);
            //leds[0] = CRGB::Black;
            //FastLED.show();
            delay(100);
        }
        // Turn on brain if VESC has been active
        if ((millis() - last_active_time) < (kbrainSleepSeconds * 1000))
        {
            set_brain_reg(true);
            //FastLED.setBrightness(200);
        }
        else
        {
            // Do some low power stuff
            //FastLED.setBrightness(10);
            set_brain_reg(false);
            //leds[0] = CRGB::Black;
            //FastLED.show();
            esp_sleep_enable_timer_wakeup(1000000); // 1 sec
            esp_light_sleep_start();
            //leds[0] = CRGB::Red;
            //FastLED.show();
            esp_sleep_enable_timer_wakeup(100000); // 1 sec
            esp_light_sleep_start();
            //leds[0] = CRGB::Black;
            //FastLED.show();
        }
    }
}
