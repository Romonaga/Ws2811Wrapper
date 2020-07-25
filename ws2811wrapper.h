#ifndef WS2811WRAPPER_H
#define WS2811WRAPPER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "ws2811.h"

#define LED_STRIPTYPE(X)                                                  \
            X(1, NEO_WS2811_STRIP_RGB, "NEO_WS2811_STRIP_RGB"),                           \
            X(2, NEO_WS2811_STRIP_RBG, "NEO_WS2811_STRIP_RBG"),                              \
            X(3, NEO_WS2811_STRIP_GRB, "NEO_WS2811_STRIP_GRB"),                                    \
            X(4, NEO_WS2811_STRIP_GBR, "NEO_WS2811_STRIP_GBR"),                        \
            X(5, NEO_WS2811_STRIP_BRG, "NEO_WS2811_STRIP_BRG"),                          \
            X(6, NEO_WS2811_STRIP_BGR, "NEO_WS2811_STRIP_BGR"),                    \
            X(7, NEO_SK6812_STRIP_RGBW, "NEO_SK6812_STRIP_RGBW"),                    \
            X(8, NEO_SK6812_STRIP_GRBW, "NEO_SK6812_STRIP_GRBW")                   \

//WS2811_STRIP_GRB
#define LED_STRIPTYPE_ENUM(type, name, str) name = type
#define LED_STRIPTYPE_STRING(type, name, str) str

typedef enum {
    LED_STRIPTYPE(LED_STRIPTYPE_ENUM),

    LED_STRIPTYPE_ENUM_COUNT
} LedStripType;


/*

#define LIGHTSCOLORS(X)                                                  \
            X(0, Nope, "Why Are We Here?",0xF0F8FF),                           \
            X(1, AliceBlue, "AliceBlue",0xF0F8FF),                           \
            X(2, AntiqueWhite, "AntiqueWhite", 0xFAEBD7),                              \
            X(3, Aqua, "Aqua",0x00FFFF),                                    \
            X(4, Aquamarine, "Aquamarine", 0x7FFFD4),                        \
            X(5, Azure, "Azure",0xF0FFFF),   \
            X(6, Beige, "Beige",0xF5F5DC),                          \
            X(7, Bisque, "Bisque",0xFFE4C4),                    \
            X(8, Black, "Black",0x000000),                    \
            X(9, Blue, "Blue",0x0000FF),                   \
            X(10, BlueViolet, "BlueViolet",0x8A2BE2),                   \
            X(11, Brown, "Brown",0xA52A2A),                   \
            X(12, BurlyWood, "BurlyWood",0xDEB887),                   \
            X(13, CadetBlue, "CadetBlue",0x5F9EA0),                   \
            X(14, Chartreuse, "Chartreuse",0x7FFF00),                   \
            X(15, Chocolate, "Chocolate",0xD2691E),                   \
            X(16, Coral, "Coral",0xFF7F50),                   \
            X(17, CornflowerBlue, "CornflowerBlue",0x6495ED)                  \


#define LIGHTSCOLORS_ENUM(type, name, twitch, color) name = type
#define LIGHTSCOLORS_TWITCH(type, name, twitch, color) twitch
#define LIGHTSCOLORS_COLOR(type, name, twitch, color) color

typedef enum {
    LIGHTSCOLORS(LIGHTSCOLORS_ENUM),

    LIGHTSCOLORS_ENUM_COUNT
} LedLightColors;*/



typedef enum {

        DarkBlue=0x00008B,
        DarkCyan=0x008B8B,
        DarkGoldenrod=0xB8860B,
        DarkGray=0xA9A9A9,
        DarkGrey=0xA9A9A9,
        DarkGreen=0x006400,
        DarkKhaki=0xBDB76B,
        DarkMagenta=0x8B008B,
        DarkOliveGreen=0x556B2F,
        DarkOrange=0xFF8C00,
        DarkOrchid=0x9932CC,
        DarkRed=0x8B0000,
        DarkSalmon=0xE9967A,
        DarkSeaGreen=0x8FBC8F,
        DarkSlateBlue=0x483D8B,
        DarkSlateGrey=0x2F4F4F,
        DarkTurquoise=0x00CED1,
        DarkViolet=0x9400D3,
        DeepPink=0xFF1493,
        DeepSkyBlue=0x00BFFF,
        DimGray=0x696969,
        DimGrey=0x696969,
        DodgerBlue=0x1E90FF,
        FireBrick=0xB22222,
        FloralWhite=0xFFFAF0,
        ForestGreen=0x228B22,
        Fuchsia=0xFF00FF,
        Gainsboro=0xDCDCDC,
        GhostWhite=0xF8F8FF,
        Gold=0xFFD700,
        Goldenrod=0xDAA520,
        Gray=0x808080,
        Grey=0x808080,
        Green=0x008000,
        GreenYellow=0xADFF2F,
        Honeydew=0xF0FFF0,
        HotPink=0xFF69B4,
        IndianRed=0xCD5C5C,
        Indigo=0x4B0082,
        Ivory=0xFFFFF0,
        Khaki=0xF0E68C,
        Lavender=0xE6E6FA,
        LavenderBlush=0xFFF0F5,
        LawnGreen=0x7CFC00,
        LemonChiffon=0xFFFACD,
        LightBlue=0xADD8E6,
        LightCoral=0xF08080,
        LightCyan=0xE0FFFF,
        LightGoldenrodYellow=0xFAFAD2,
        LightGreen=0x90EE90,
        LightGrey=0xD3D3D3,
        LightPink=0xFFB6C1,
        LightSalmon=0xFFA07A,
        LightSeaGreen=0x20B2AA,
        LightSkyBlue=0x87CEFA,
        LightSlateGray=0x778899,
        LightSlateGrey=0x778899,
        LightSteelBlue=0xB0C4DE,
        LightYellow=0xFFFFE0,
        Lime=0x00FF00,
        LimeGreen=0x32CD32,
        Linen=0xFAF0E6,
        Magenta=0xFF00FF,
        Maroon=0x800000,
        MediumAquamarine=0x66CDAA,
        MediumBlue=0x0000CD,
        MediumOrchid=0xBA55D3,
        MediumPurple=0x9370DB,
        MediumSeaGreen=0x3CB371,
        MediumSlateBlue=0x7B68EE,
        MediumSpringGreen=0x00FA9A,
        MediumTurquoise=0x48D1CC,
        MediumVioletRed=0xC71585,
        MidnightBlue=0x191970,
        MintCream=0xF5FFFA,
        MistyRose=0xFFE4E1,
        Moccasin=0xFFE4B5,
        NavajoWhite=0xFFDEAD,
        Navy=0x000080,
        OldLace=0xFDF5E6,
        Olive=0x808000,
        OliveDrab=0x6B8E23,
        Orange=0xFFA500,
        OrangeRed=0xFF4500,
        Orchid=0xDA70D6,
        PaleGoldenrod=0xEEE8AA,
        PaleGreen=0x98FB98,
        PaleTurquoise=0xAFEEEE,
        PaleVioletRed=0xDB7093,
        PapayaWhip=0xFFEFD5,
        PeachPuff=0xFFDAB9,
        Peru=0xCD853F,
        Pink=0xFFC0CB,
        Plaid=0xCC5533,
        Plum=0xDDA0DD,
        PowderBlue=0xB0E0E6,
        Purple=0x800080,
        Red=0xFF0000,
        RosyBrown=0xBC8F8F,
        RoyalBlue=0x4169E1,
        SaddleBrown=0x8B4513,
        Salmon=0xFA8072,
        SandyBrown=0xF4A460,
        SeaGreen=0x2E8B57,
        Seashell=0xFFF5EE,
        Sienna=0xA0522D,
        Silver=0xC0C0C0,
        SkyBlue=0x87CEEB,
        SlateBlue=0x6A5ACD,
        SlateGray=0x708090,
        SlateGrey=0x708090,
        Snow=0xFFFAFA,
        SpringGreen=0x00FF7F,
        SteelBlue=0x4682B4,
        Tan=0xD2B48C,
        Teal=0x008080,
        Thistle=0xD8BFD8,
        Tomato=0xFF6347,
        Turquoise=0x40E0D0,
        Violet=0xEE82EE,
        Wheat=0xF5DEB3,
        White=0xFFFFFF,
        WhiteSmoke=0xF5F5F5,
        Yellow=0xFFFF00,
        YellowGreen=0x9ACD32,

        // LED RGB color that roughly approximates
        // the color of incandescent fairy lights,
        // assuming that you're using FastLED
        // color correction on your LEDs (recommended).
        FairyLight=0xFFE42D,
        // If you are using no color correction, use this
        FairyLightNCC=0xFF9D2A

    } HTMLColorCode;


class  Ws2811Wrapper
{
public:


public:
    //Constructor destructor (Bet you know that)
    Ws2811Wrapper();
    ~Ws2811Wrapper();

    //You Must Init The Matrix before use
    ws2811_return_t initStrip(u_int32_t rows, u_int32_t columns, LedStripType stripType, int dma, int gpio);

    //Clears the strip (Turns LEDS off.
    ws2811_return_t clearLeds(bool render = true);

    //void matrix_raise(void);

    // This Will Move the Matric into the leds for rendering with core
    ws2811_return_t show();

    // Self commenting code here
    void setPixelColor(u_int32_t row, u_int32_t pixal, ws2811_led_t color);
    void setPixelColor(u_int32_t pixal, ws2811_led_t color);
    void setPixelColor(ws2811_led_t color);
    void setPixelColor(u_int32_t row, u_int32_t pixal, u_int8_t red, u_int8_t green, u_int8_t blue);
    void setPixelColor(u_int32_t row, u_int32_t pixal, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white);
    ws2811_led_t getPixelColor(u_int32_t row, u_int32_t pixal);
    ws2811_led_t getPixelColor(u_int32_t pixal);


    // You can only specify for the complete strip.
    // Some LEDStrips all for a whitness (think luminosoty(sic))
    void setBrightness(u_int8_t brightness);


    // You can only specify for the complete strip.
    // Colors are funny, gamma correction attempts to
    //make colors more like the eye sees them.
    void setCustomGammaCorrection(double gammaFactor);

    //You can shut down the Matrix but leave  the lights on.
    //This allows for that in the destructor, the defailt is yes.
    //When the destroctor fires and this is set to false The strip will
    //not set the leds of off (I simply call the clear_leds();
    void setClearOnExit(bool clear);

    //Used for matrix that alernates rows.
    void matrixraise(void);

    //Self commenting code
    u_int32_t getNumberLeds();
    u_int32_t getColumns();
    u_int32_t getRows();


    //On functions that return a ws2811_return_t, this will return a human readable error code back.
    static const char * ws2811_get_return_t_str(const ws2811_return_t state);

    //Used for converting the RGB into a useul number for the matrix colors.
    //These staic methods are helps as inputs to
    static ws2811_led_t Color(u_int8_t red, u_int8_t green, u_int8_t blue);
    static ws2811_led_t Color(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white);
    static ws2811_led_t Wheel(u_int8_t wheelPos);
    static int Red(ws2811_led_t color);
    static int  Green(ws2811_led_t color);
    static int  Blue(ws2811_led_t color);
    static ws2811_led_t DimColor(ws2811_led_t color);
    static ws2811_led_t BrightenColor(ws2811_led_t color);



    static void waitSec(u_int32_t sec);
    static void waitMillSec(u_int32_t mill);

private:
    //Internal
    void setStripType();
    void cleanUp();



    //Private is private we don't talk about them.
private:
    ws2811_led_t *_matrix;
    ws2811_t    _ledstring;
    int _dma;
    int _gpio;
    bool _useGamaCorrection;
    u_int32_t _numLights;
    u_int32_t _columns;
    u_int32_t _rows;
    LedStripType _stripType;
    bool _clearOnExit;

};

#endif // WS2811WRAPPER_H
//https://bisqwit.iki.fi/story/howto/dither/jy/
/*
 * <?php

 //Create a 8x8 threshold map
$map = array_map(function($p)
                 {
                   $q = $p ^ ($p >> 3);
                   return ((($p & 4) >> 2) | (($q & 4) >> 1)
                         | (($p & 2) << 1) | (($q & 2) << 2)
                         | (($p & 1) << 4) | (($q & 1) << 5)) / 64.0;
                 }, range(0,63));

// Define palette
$pal = Array(0x080000,0x201A0B,0x432817,0x492910,
             0x234309,0x5D4F1E,0x9C6B20,0xA9220F,
             0x2B347C,0x2B7409,0xD0CA40,0xE8A077,
             0x6A94AB,0xD5C4B3,0xFCE76E,0xFCFAE2);

//Read input image
$srcim = ImageCreateFromPng('scene.png');
$w = ImageSx($srcim);
$h = ImageSy($srcim);

// Create paletted image
$im = ImageCreate($w,$h);
foreach($pal as $c) ImageColorAllocate($im, $c>>16, ($c>>8)&0xFF, $c&0xFF);

$thresholds = Array(256/4, 256/4, 256/4);

// Render the paletted image by converting each input pixel using the threshold map.
for($y=0; $y<$h; ++$y)
  for($x=0; $x<$w; ++$x)
  {
    $map_value = $map[($x & 7) + (($y & 7) << 3)];
    $color = ImageColorsForIndex($srcim, ImageColorAt($srcim, $x,$y));
    $r = (int)($color['red']   + $map_value * $thresholds[0]);
    $g = (int)($color['green'] + $map_value * $thresholds[1]);
    $b = (int)($color['blue']  + $map_value * $thresholds[2]);
    // Plot using the palette index with color that is closest to this value
    ImageSetPixel($im, $x,$y, ImageColorClosest($im, $r,$g,$b));
  }
ImagePng($im, 'scenebayer0.png');
 *
 * */
