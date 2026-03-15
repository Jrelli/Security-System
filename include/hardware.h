#ifndef HARDWARE_H
#define HARDWARE_H

#include <Adafruit_GFX.h> // graphics for screen
#include <Adafruit_SSD1306.h> // driver chip for screen

/* PIN DEFINITIONS */
#define PIN_REED_SWITCH_NC  13

/* SCREEN DEFINITIONS */
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64

/* VARIABLES */
extern Adafruit_SSD1306 display;

/* FUNCTIONS */
/// @brief turns on the screen & posts the booting page
void setupScreen();

/// @brief posts the main page with updated data
void updateScreen();

/// @brief Gets the status of the reed switch (door)
/// @return Returns true if the door is open and false if the door is closed
bool getDoorStatus();

/// @brief Gets the status of the Wi-Fi connection
/// @return Returns true if we are connected to the Wi-Fi
bool getWiFiStatus();

/// @brief Gets the status of the web server page
/// @return Returns true if the web page is up and responding
bool getServerStatus();

/// @brief Gets the percentage of the battery using a LUT (Look Up Table)
/// @return Returns the battery percentage as an integer (0-100)
int getBatteryPercent();

/// @brief Gets the battery voltage from twenty readings averaged.
/// @return Returns the battery voltage as a float
float getBatteryVoltage();

#endif