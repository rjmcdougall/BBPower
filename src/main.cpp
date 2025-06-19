
#include <Arduino.h>
#include <FastLED.h>
#include "burner_power.h"
//#include "bms_can.h"
#include "ina229.h"
//#include <Adafruit_ST7789.h>
//#include <Fonts/FreeSans12pt7b.h>
static const int kbrainSleepSeconds = 100000;

#ifdef ESP8266
#define HEADLIGHT_ON 12
#define LEG_REG_ENABLE 13
#define REG_5V_ENABLE 15
#define LEG_REG_RELAY_OFF 16
#define AMP_RELAY_OFF 0
#else
#define HEADLIGHT_ON 12
#define LEG_REG_DISABLE 11
#define REG_5V_DISABLE 10
#define LEG_REG_RELAY_OFF 9
#define AMP_RELAY_OFF 6
#endif

//#define NEOPIXEL_POWER 21
//#define PIN_NEOPIXEL 33
#define NEOPIXEL_POWER_ON 1

#define LED_BUILTIN 13

static constexpr const char *TAG = "main";

//#define TFT_RST 0

/*
#define TFT_CS 7
#define TFT_DC 39
#define TFT_RST 40
#define TFT_I2C_POWER 21
#define TFT_BACKLITE 45
*/

//Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 t = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

bms_can can; // Canbus
ina229 ina;

CRGB leds[1];

//GFXcanvas16 canvas(240, 135);

void setup()
{
    delay(1000);
    Serial.begin(460800);
    /*

    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);

    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);

    //Serial.println("tft init");
    tft.setRotation(3);
    //tft.setFont(&FreeSans12pt7b);
    tft.setTextSize(3);
    tft.fillScreen(ST77XX_RED);
    tft.setCursor(20, 50);
    tft.setTextColor(ST77XX_WHITE);
    tft.println("Burner Board");


    Serial.println("starting");

    Serial.print("TFT_CS ");
    Serial.println(TFT_CS);
    Serial.print("TFT_DC ");
    Serial.println(TFT_DC);
    Serial.print("TFT_RST ");
    Serial.println(TFT_RST);
    Serial.print("TFT_I2C_POWER ");
    Serial.println(TFT_I2C_POWER);
    Serial.print("TFT_BACKLITE ");
    Serial.println(TFT_BACKLITE);

    */

    
    FastLED.addLeds<NEOPIXEL, PIN_NEOPIXEL>(leds, 1);

    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);

    leds[0] = CRGB::Red;
    FastLED.show();
    

/*
    Serial.println("starting tft power");

    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);

    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
    delay(1000);

    Serial.println("starting tft");
    //tft.init(135, 240, SPI_MODE0); // Init ST7789 240x135
    tft.init(135, 240); // Init ST7789 240x135
    delay(100);

    Serial.println("tft init");
    tft.setRotation(3);
    //tft.setFont(&FreeSans12pt7b);
    tft.setTextSize(3);
    tft.fillScreen(ST77XX_RED);
    tft.setCursor(20, 50);
    tft.setTextColor(ST77XX_WHITE);
    tft.println("Burner Board");
    Serial.println("tft init done");
    //tft.end();

    delay(1000);
    */

    Serial.println("starting canbus...");
    can.begin();
    ina.begin();
    delay(100);

    ina.setShunt(0.001f);
    ina.setShuntCal(1.0f);

    BLog_d(TAG, "ina229 manf-id = %x", ina.ReadManufacturerID());
    BLog_d(TAG, "ina229 devid = %x", ina.ReadDeviceID());
    BLog_d(TAG, "ina229 voltage = %f", ina.vBus());
    BLog_d(TAG, "ina229 current = %f", ina.current());
    BLog_d(TAG, "ina229 temperature = %f", ina.dieTemp());
    


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

    delay(1000);
    
}

uint32_t last_active_time = 0;

void loop()
{

    
    delay(100);
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
    
 

    float voltage = ina.vBus();
    float current = ina.current();
    float temp = ina.dieTemp();

    BLog_d(TAG, "vesc = %d, rpm = %f, ina229 vbus = %f, vshunt = %f mv, current = %f", can.vescActive(), can.vescRpm(), ina.vBus(), ina.vShunt() * 1000, ina.current());
    delay(100);
    

    //ina.end();

    //tft.init(135, 240); // Init ST7789 240x135
    //tft.begin();
    //tft.initSPI(1000000, SPI_MODE0);


    /*
    tft.setRotation(3);
    tft.setTextSize(3);
    tft.fillScreen(ST77XX_GREEN);
    tft.setCursor(20, 50);
    tft.setTextColor(ST77XX_WHITE);
    tft.println("Burner Board");
    */


    //tft.setCursor(0, 0);
    //tft.print(voltage);
    //tft.println("v");
    //tft.print(current);
    //tft.setCursor(0, 10);
    //tft.println("a");  

    
    delay(100);
    leds[0] = CRGB::Green;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    


    
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
        delay(10);
        digitalWrite(LEG_REG_DISABLE, LOW);
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(100);
    }
    else
    {
        leds[0] = CRGB::Green;
        FastLED.show();
        digitalWrite(LEG_REG_DISABLE, HIGH);
        delay(10);
        digitalWrite(AMP_RELAY_OFF, HIGH);
        digitalWrite(LEG_REG_RELAY_OFF, HIGH);
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(100);
    }
    // Turn on brain if VESC has been active
    if ((millis() - last_active_time) < (kbrainSleepSeconds * 1000))
    {
        digitalWrite(REG_5V_DISABLE, LOW);
        FastLED.setBrightness(200);
    }
    else
    {
        // Do some low power stuff
        FastLED.setBrightness(10);
        digitalWrite(REG_5V_DISABLE, HIGH);
        leds[0] = CRGB::Black;
        FastLED.show();
        esp_sleep_enable_timer_wakeup(1000000); // 1 sec
        esp_light_sleep_start();
        leds[0] = CRGB::Red;
        FastLED.show();
        esp_sleep_enable_timer_wakeup(100000); // 1 sec
        esp_light_sleep_start();
        leds[0] = CRGB::Black;
        FastLED.show();
    }
    
}

/*
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
    digitalWrite(LEG_REG_DISABLE, LOW);
    delay(1000);
    digitalWrite(REG_5V_DISABLE, LOW);
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
    digitalWrite(LEG_REG_DISABLE, HIGH);
    digitalWrite(AMP_RELAY_OFF, HIGH);
    digitalWrite(REG_5V_DISABLE, LOW);
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
*/