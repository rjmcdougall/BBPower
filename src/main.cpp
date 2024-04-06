
#include <Arduino.h>
#include <FastLED.h>
#include "burner_power.h"
#include "bms_can.h"
#include "ina229.h"

#ifdef ESP8266
#define HEADLIGHT_ON 12
#define LEG_REG_ENABLE 13
#define REG_5V_ENABLE 15
#define LEG_REG_RELAY_OFF 16
#define AMP_RELAY_OFF 0
#else
#define HEADLIGHT_ON 12
#define LEG_REG_ENABLE 11
#define REG_5V_ENABLE 10
#define LEG_REG_RELAY_OFF 9
#define AMP_RELAY_OFF 6
#endif

#define NEOPIXEL_POWER 21
#define PIN_NEOPIXEL 33
#define NEOPIXEL_POWER_ON 1

#define LED_BUILTIN 13

static constexpr const char *TAG = "main";

bms_can can; // Canbus
ina229 ina;

CRGB leds[1];

void setup()
{
    Serial.begin(460800);

    Serial.println("starting");

    can.begin();
    ina.begin();
    ina.setShunt(0.001f);
    ina.setShuntCal(1.0f);

    BLog_d(TAG, "ina229 manf-id = %x", ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 devid = %x", ina.ReadDeviceID());
    BLog_d(TAG, "ina229 voltage = %f", ina.vBus());
    BLog_d(TAG, "ina229 current = %f", ina.current());
    BLog_d(TAG, "ina229 temperature = %f", ina.dieTemp());

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    FastLED.addLeds<NEOPIXEL, PIN_NEOPIXEL>(leds, 1);

    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);

    leds[0] = CRGB::Red;
    FastLED.show();

    pinMode(HEADLIGHT_ON, OUTPUT);
    digitalWrite(HEADLIGHT_ON, LOW);

    pinMode(LEG_REG_RELAY_OFF, OUTPUT); // LED REG RELAY HIGH=OFF
    digitalWrite(LEG_REG_RELAY_OFF, HIGH);

    pinMode(AMP_RELAY_OFF, OUTPUT); // AMP RELAY HIGH=OFF
    digitalWrite(AMP_RELAY_OFF, HIGH);

    pinMode(LEG_REG_ENABLE, OUTPUT); // LED_REG ENABLE HIGH=ON
    digitalWrite(LEG_REG_ENABLE, LOW);

    pinMode(REG_5V_ENABLE, OUTPUT); // 5V REG ENABLLE HIGH=ON
    digitalWrite(REG_5V_ENABLE, LOW);

    delay(5000);
}

uint32_t last_active_time = 0;

void loop()
{
    BLog_d(TAG, "vesc = %d, rpm = %f, ina229 vbus = %f, vshunt = %f mv, current = %f", can.vescActive(), can.vescRpm(), ina.vBus(), ina.vShunt() * 1000, ina.current());
    if (can.vescRpm() > 0)
    {
        digitalWrite(HEADLIGHT_ON, HIGH);
    }
    else
    {
        digitalWrite(HEADLIGHT_ON, LOW);
    }

    if (can.vescActive())
    {
        last_active_time = millis();
        leds[0] = CRGB::Blue;
        FastLED.show();
        digitalWrite(AMP_RELAY_OFF, LOW);
        digitalWrite(LEG_REG_RELAY_OFF, LOW);
        delay(100);
        digitalWrite(LEG_REG_ENABLE, HIGH);
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(50);
    }
    else
    {
        leds[0] = CRGB::Green;
        FastLED.show();
        digitalWrite(LEG_REG_ENABLE, LOW);
        delay(100);
        digitalWrite(AMP_RELAY_OFF, HIGH);
        digitalWrite(LEG_REG_RELAY_OFF, HIGH);
        leds[0] = CRGB::Black;
        FastLED.show();
    }
    if ((millis() - last_active_time) < 10000)
    {
        digitalWrite(REG_5V_ENABLE, HIGH);
        FastLED.setBrightness(200);
    }
    else
    {
        FastLED.setBrightness(10);
        digitalWrite(REG_5V_ENABLE, LOW);
        leds[0] = CRGB::Black;
        FastLED.show();
        esp_sleep_enable_timer_wakeup(1000000); // 1 sec
        esp_light_sleep_start();
        leds[0] = CRGB::Red;
        FastLED.show();
        esp_sleep_enable_timer_wakeup(100000); // 1 sec
        esp_light_sleep_start();
    }
}

void loop2()
{

    Serial.println("relays on");
    leds[0] = CRGB::White;
    FastLED.show();

    digitalWrite(AMP_RELAY_OFF, LOW);
    delay(500);
    digitalWrite(LEG_REG_RELAY_OFF, LOW);
    BLog_d(TAG, "ina229 id = %d", (uint32_t)ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 voltage = %d", ina.GetVBus());
    BLog_d(TAG, "ina229 current = %d", ina.current());
    delay(500);

    Serial.println("regs on");

    leds[0] = CRGB::Green;
    FastLED.show();
    for (int i = 0; i < 10; i++)
    {
        delay(1000);
        BLog_d(TAG, "ina229 current = %d", ina.current());
    }
    digitalWrite(LEG_REG_ENABLE, HIGH);
    delay(1000);
    digitalWrite(REG_5V_ENABLE, HIGH);
    delay(1000);
    digitalWrite(HEADLIGHT_ON, HIGH);
    BLog_d(TAG, "ina229 id = %d", (uint32_t)ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 voltage = %d", ina.GetVBus());
    BLog_d(TAG, "ina229 current = %d", ina.current());

    for (int i = 0; i < 10; i++)
    {
        delay(1000);
        BLog_d(TAG, "ina229 current = %d", ina.current());
    }

    Serial.println("devices low power + brain");

    leds[0] = CRGB::Blue;
    FastLED.show();
    digitalWrite(LEG_REG_ENABLE, LOW);
    digitalWrite(AMP_RELAY_OFF, HIGH);
    digitalWrite(REG_5V_ENABLE, HIGH);
    digitalWrite(HEADLIGHT_ON, LOW);
    delay(500);
    digitalWrite(LEG_REG_RELAY_OFF, HIGH);
    BLog_d(TAG, "ina229 id = %d", (uint32_t)ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 voltage = %d", ina.GetVBus());
    BLog_d(TAG, "ina229 current = %d", ina.current());
    for (int i = 0; i < 30; i++)
    {
        delay(1000);
        BLog_d(TAG, "ina229 current = %d", ina.current());
    }
    Serial.println("devices off");

    leds[0] = CRGB::Purple;
    FastLED.show();
    digitalWrite(HEADLIGHT_ON, LOW);
    digitalWrite(LEG_REG_RELAY_OFF, HIGH);
    digitalWrite(AMP_RELAY_OFF, HIGH);
    BLog_d(TAG, "ina229 id = %d", (uint32_t)ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 voltage = %d", ina.GetVBus());
    BLog_d(TAG, "ina229 current = %d", ina.current());
    for (int i = 0; i < 10; i++)
    {
        delay(1000);
        BLog_d(TAG, "ina229 current = %d", ina.current());
    }
}