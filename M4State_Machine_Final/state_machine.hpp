#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define RGB_CONTROL                     PIN_NEOPIXEL            // DIO 8 on feather M4 CAN
#define red 16, 0, 0
#define green 0, 16, 0
#define blue 0, 0, 16
#define cyan 0, 16, 16
#define yellow 16, 16, 0
#define magenta 16, 0, 16
#define exp(r, g, b) r, g, b
#define setled(color)  rgbled.setPixelColor(0, rgbled.Color(exp(color))); rgbled.show()

// Convert from 3.3V to 5V range
float convert_to_psi(uint32_t x);
float convert_to_travel_APPS0(uint32_t x);
float convert_to_travel_APPS1(uint32_t x);
uint32_t convert_to_uint32(float x, float y);
void wait_function (int x);