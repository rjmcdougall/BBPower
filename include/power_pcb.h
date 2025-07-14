
// Public interface for controlling the Burner project's power PCB.
// This header defines the functions for managing lights, relays, and regulators.

#ifndef POWER_PCB_H
#define POWER_PCB_H

// Include the core Arduino library for standard types and functions
#include <Arduino.h>


#define FEATHERESP32 1

// Define hardware pin assignments based on the compilation target.
#if defined(ESP8266)
    #define LED_BUILTIN 13
    #define HEADLIGHT_ON        12
    #define LEG_REG_ENABLE      13  
    #define REG_5V_ENABLE       15  
    #define LEG_REG_RELAY_OFF   16
    #define AMP_RELAY_OFF       0
    #define AMP_RELAY_OFF       11
    #define FUD_BUTTON          16
#elif defined(FEATHERESP32) // 
    #define LED_BUILTIN 13
    #define INA_CS              gpio_num_t::GPIO_NUM_4
    #define CANBUS_TX           gpio_num_t::GPIO_NUM_17
    #define CANBUS_RX           gpio_num_t::GPIO_NUM_16
    #define LEG_REG_RELAY_OFF   32
    #define AMP_RELAY_OFF       15
    #define REG_5V_DISABLE      33
    #define LEG_REG_DISABLE     27
    #define HEADLIGHT_ON        12
    #define FUD_BUTTON          2
#elif defined(ESP32S3) // 
    #define LED_BUILTIN 13
    #define HEADLIGHT_ON        12
    #define LEG_REG_DISABLE     11
    #define REG_5V_DISABLE      10
    #define LEG_REG_RELAY_OFF   6
    #define AMP_RELAY_OFF       9
    #define FUD_BUTTON          16
#elif defined(ESP32S2) // Defaulting to ESP32 or other boards
    #define LED_BUILTIN 13
    #define HEADLIGHT_ON        12
    #define LEG_REG_DISABLE     11
    #define REG_5V_DISABLE      10
    #define LEG_REG_RELAY_OFF   6
    #define AMP_RELAY_OFF       9
    #define FUD_BUTTON          16
#elif defined(ESP32S3tft) 
    #define INA_CS              gpio_num_t::GPIO_NUM_8
    #define LED_BUILTIN 13
    #define HEADLIGHT_ON        12
    #define LEG_REG_DISABLE     11
    #define REG_5V_DISABLE      10
    #define LEG_REG_RELAY_OFF   6
    #define AMP_RELAY_OFF       9
    #define FUD_BUTTON          16
#else  
    DIE
#endif

/**
 * @brief Initializes all pins for the power PCB to a safe default state.
 * Must be called once in the main setup() function.
 */
void powerpcb_init();

/**
 * @brief Turns the main headlight on or off.
 * @param state true for ON, false for OFF.
 */
void set_headlight(boolean state);

/**
 * @brief Enables or disables the 5V regulator for the main "brain" components.
 * @param state true to ENABLE the regulator, false to DISABLE.
 */
void set_brain_reg(boolean state);

/**
 * @brief Enables or disables the voltage regulator for the leg LEDs.
 * @param state true to ENABLE the regulator, false to DISABLE.
 */
void set_led_reg(boolean state);

/**
 * @brief Controls the master power relay for the LEDs.
 * @param state true for ON, false for OFF.
 */
void set_led_master(boolean state);

/**
 * @brief Controls the auto power states by vesc state.
 * @param state true for ON, false for OFF.
 */
void set_auto(boolean state);


/**
 * @brief Controls the power relay for the audio amplifier.
 * @param state true for ON, false for OFF.
 */
void set_amp(boolean state);

/**
 * @brief Gets the last known commanded state of the headlight.
 * @return The current state (true for ON, false for OFF).
 */
boolean get_headlight();

/**
 * @brief Gets the last known commanded state of the 5V "brain" regulator.
 * @return The current state (true for ENABLED, false for DISABLED).
 */
boolean get_brain_reg();

/**
 * @brief Gets the last known commanded state of the LED voltage regulator.
 * @return The current state (true for ENABLED, false for DISABLED).
 */
boolean get_led_reg();

/**
 * @brief Gets the last known commanded state of the LED master power relay.
 * @return The current state (true for ON, false for OFF).
 */
boolean get_led_master();

/**
 * @brief Gets the last known commanded state of the auto mode.
 * @return The current state (true for ON, false for OFF).
 */
boolean get_auto();


/**
 * @brief Gets the last known commanded state of the amplifier power relay.
 * @return The current state (true for ON, false for OFF).
 */
boolean get_amp();

/**
 * @brief Gets the state of the fud button.
 * @return The current state (true for PRESSED, false for NOT PRESSED).
 */
boolean get_fud_button();



#endif // POWER_PCB_H
