#ifndef WS2811WRAPPER_H
#define WS2811WRAPPER_H

//#include "ws2811wrapper_global.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "ws2811.h"

#define LED_STRIPTYPE(X)                                                  \
            X(1, NEO_WS2811_STRIP_RGB, "Unknown Device."),                           \
            X(2, NEO_WS2811_STRIP_RBG, "Sensor Device"),                              \
            X(3, NEO_WS2811_STRIP_GRB, "Led Device"),                                    \
            X(4, NEO_WS2811_STRIP_GBR, "Monitoring Deviice"),                        \
            X(5, NEO_WS2811_STRIP_BRG, "Security Device"),                          \
            X(6, NEO_WS2811_STRIP_BGR, "Weather Station Device"),                    \
            X(7, NEO_SK6812_STRIP_RGBW, "Simple Blink Test Device"),                    \
            X(8, NEO_SK6812_STRIP_GRBW, "Fire Detector Device")                   \


#define LED_STRIPTYPE_ENUM(type, name, str) name = type
#define LED_STRIPTYPE_STRING(type, name, str) str

typedef enum {
    LED_STRIPTYPE(LED_STRIPTYPE_ENUM),

    LED_STRIPTYPE_ENUM_COUNT
} LedStripType;



class  Ws2811Wrapper
{
public:


public:
    //Constructor destructor (Bet you know that)
    Ws2811Wrapper();
    ~Ws2811Wrapper();

    //You Must Init The Matrix before use
    ws2811_return_t initStrip(int width, int hight, LedStripType stripType, int dma, int gpio);

    //Clears the strip (Turns LEDS off.
    ws2811_return_t clearLeds(bool render = true);

    //void matrix_raise(void);

    // This Will Move the Matric into the leds for rendering with core
    ws2811_return_t show();

    // Selv commenting code here
    void setPixelColor(u_int32_t width, u_int32_t hight, ws2811_led_t color);
    void setPixelColor(ws2811_led_t color);
    void setPixelColor(u_int32_t width, u_int32_t hight, u_int8_t red, u_int8_t green, u_int8_t blue);
    void setPixelColor(u_int32_t width, u_int32_t hight, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white);
    ws2811_led_t getPixelColor(u_int32_t width, u_int32_t hight);

    // You can only specify for the complete strip.
    // Some LEDStrips all for a whitness (think luminosoty(sic))
    void setBrightness(u_int8_t brightness);

    //You can shut down the Matrix but leave  the lights on.
    //This allows for that in the destructor, the defailt is yes.
    //When the destroctor fires and this is set to false The strip will
    //not set the leds of off (I simply call the clear_leds();
    void setClearOnExit(bool clear);

    //Self commenting code
    u_int32_t getNumberLeds();

    //On functions that return a ws2811_return_t, this will return a human readable error code back.
    static const char * ws2811_get_return_t_str(const ws2811_return_t state);

    //Used for converting the RGB into a useul number for the matrix colors.
    //These staic methods are helps as inputs to
    static ws2811_led_t Color(u_int8_t red, u_int8_t green, u_int8_t blue);
    static ws2811_led_t Color(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white);
    static ws2811_led_t Wheel(u_int8_t wheelPos);

    static void waitSec(u_int32_t sec);
    static void waitMillSec(u_int32_t mill);

private:
    //Internal
    void setStripType();

    //Private is private we don't talk about them.
private:
    ws2811_led_t *_matrix;
    ws2811_t    _ledstring;
    int _dma;
    int _gpio;
    int _numLights;
    int _height;
    int _width;
    LedStripType _stripType;
    bool _clearOnExit;

};

#endif // WS2811WRAPPER_H
