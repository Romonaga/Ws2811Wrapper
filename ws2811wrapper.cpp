#include "ws2811wrapper.h"

#include <cstring>
#include <time.h>


 uint8_t  gamma8[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 5, 6, 6, 6, 6, 7, 7, 7,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 5, 5, 5, 7, 8, 8, 8, 9, 9, 9,10,
10, 10, 11, 11, 11, 12, 12, 13,
17, 17, 18, 18, 19, 19, 20, 20,
25, 26, 27, 27, 28, 29, 29, 30,
37, 38, 39, 39, 40, 41, 42, 43,
51, 52, 54, 55, 56, 57, 58, 59,
69, 70, 72, 73, 74, 75, 77, 78,
90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142, 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175, 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213, 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint8_t  gammadefalt[256];


Ws2811Wrapper::Ws2811Wrapper()
{
    std::memset(&_ledstring, '\0', sizeof(_ledstring));
    _ledstring.freq = WS2811_TARGET_FREQ;
    _ledstring.dmanum =  0;

    _ledstring.channel[0].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[0].gpionum = 0;
    _ledstring.channel[0].count = 0;
    _ledstring.channel[0].invert = 0;
    _ledstring.channel[0].brightness = 255;
    _ledstring.channel[0].leds = nullptr;
    _ledstring.channel[0].gamma = nullptr;


    _ledstring.channel[1].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[1].gpionum = 0;
    _ledstring.channel[1].count = 0;
    _ledstring.channel[1].invert = 0;
    _ledstring.channel[1].brightness = 255;
    _ledstring.channel[1].leds = nullptr;
    _ledstring.channel[1].gamma = nullptr;

    _height = 1;
    _width = 0;
    _useGamaCorrection = false;
    _clearOnExit = true;
    for(int counter = 0; counter < 256; counter++)
    {
        gammadefalt[counter] = counter;
    }
    _matrix = NULL;
}

Ws2811Wrapper::~Ws2811Wrapper()
{
    if(_matrix != NULL)
    {
        if(true == _clearOnExit)
            clearLeds();

        //the ws2811c allocates a memory buffer
        //as we wish to override it we will insure memory
        //is nulled before we finalize.
        _ledstring.channel[1].gamma = nullptr;
        _ledstring.channel[0].gamma = nullptr;

        ws2811_fini(&_ledstring);
        delete [] _matrix;
    }

}

ws2811_return_t Ws2811Wrapper::show()
{
    ws2811_return_t result = WS2811_SUCCESS;

    u_int32_t x, y;

    for (x = 0; x < _width; x++)
       for (y = 0; y < _height; y++)
           _ledstring.channel[0].leds[(y * _width) + x] = _matrix[ (y * _width) + x];

    result = ws2811_render(&_ledstring);

    if(result != WS2811_SUCCESS)
        fprintf(stderr , "Ws2811Wrapper show Result(%s)", ws2811_get_return_t_str(result));

    return result;
}

void Ws2811Wrapper::setCustomGammaCorrection(uint8_t*  gamma8)
{
    _ledstring.channel[1].gamma = gamma8;
    _ledstring.channel[0].gamma = gamma8;
}

void Ws2811Wrapper::setGammaCorrection(bool useGammaCorrection)
{
    _useGamaCorrection = useGammaCorrection;
    if(true == _useGamaCorrection)
    {
        _ledstring.channel[1].gamma = gamma8;
        _ledstring.channel[0].gamma = gamma8;
    }
    else
    {
        _ledstring.channel[1].gamma = gammadefalt;
        _ledstring.channel[0].gamma = gammadefalt;
    }
}

ws2811_return_t Ws2811Wrapper::initStrip(u_int32_t width, u_int32_t height, LedStripType stripType, int dma, int gpio, bool useGamaCorrection)
{
    ws2811_return_t retval = WS2811_SUCCESS;

    _height = height;
    _width = width;
    _ledstring.channel[0].count = _width * _height;

    _stripType = stripType;
    setStripType();

    switch (gpio)
    {
        case 12:
        case 18:
        case 40:
        case 52:
            _ledstring.channel[0].gpionum = gpio;
            break;

        default:
            retval = WS2811_ERROR_GPIO_INIT;
            break;
    }

    if (dma < 14)
    {
        _ledstring.dmanum = dma;

    }
    else
    {
        retval = WS2811_ERROR_DMA;

    }

    if (retval == WS2811_SUCCESS)
    {
         _matrix =  new ws2811_led_t[sizeof(ws2811_led_t) * _width * _height];
        retval =  ws2811_init(&_ledstring);
    }

    //the ws2811c allocates a memory buffer
    //as we wish to override it this hack will deallocate
    //and replace with our own gamma's I prefered this over modifying
    //the ws2811c lib.

    free(_ledstring.channel[1].gamma);
    free(_ledstring.channel[0].gamma);

    setGammaCorrection(useGamaCorrection);

    return retval;
}

void Ws2811Wrapper::setStripType()
{
    switch(_stripType)
    {

    case NEO_WS2811_STRIP_RGB:
        _ledstring.channel[0].strip_type = WS2811_STRIP_RGB;
        _ledstring.channel[1].strip_type = WS2811_STRIP_RGB;
        break;

    case NEO_WS2811_STRIP_RBG:
        _ledstring.channel[0].strip_type = WS2811_STRIP_RBG;
        _ledstring.channel[1].strip_type = WS2811_STRIP_RBG;
        break;

    case NEO_WS2811_STRIP_GRB:
        _ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
        _ledstring.channel[1].strip_type = WS2811_STRIP_GRB;
        break;

    case NEO_WS2811_STRIP_GBR:
        _ledstring.channel[0].strip_type = WS2811_STRIP_GBR;
        _ledstring.channel[1].strip_type = WS2811_STRIP_GBR;

        break;

    case NEO_WS2811_STRIP_BRG:
        _ledstring.channel[0].strip_type = WS2811_STRIP_BRG;
        _ledstring.channel[1].strip_type = WS2811_STRIP_BRG;
        break;

    case NEO_WS2811_STRIP_BGR:
        _ledstring.channel[0].strip_type = WS2811_STRIP_BGR;
        _ledstring.channel[1].strip_type = WS2811_STRIP_BGR;
        break;

    case NEO_SK6812_STRIP_RGBW:
        _ledstring.channel[0].strip_type = SK6812_STRIP_RGBW;
        _ledstring.channel[1].strip_type = SK6812_STRIP_RGBW;
        break;

    case NEO_SK6812_STRIP_GRBW:
        _ledstring.channel[0].strip_type = SK6812_STRIP_GRBW;
        _ledstring.channel[1].strip_type = SK6812_STRIP_GRBW;
        break;

    default:
        //todo logerror
        break;
    }
}

const char * Ws2811Wrapper::ws2811_get_return_t_str(const ws2811_return_t state)
{
    const unsigned int index = -state;
    static const char * const ret_state_str[] = { WS2811_RETURN_STATES(WS2811_RETURN_STATES_STRING) };

    if (index < sizeof(ret_state_str) / sizeof(ret_state_str[0]))
    {
        return ret_state_str[index];
    }

    return "";
}


ws2811_return_t Ws2811Wrapper::clearLeds(bool render)
{
    ws2811_return_t retVal = WS2811_SUCCESS;

    u_int32_t x, y;

    for (y = 0; y < _height ; y++)
    {
        for (x = 0; x < _width; x++)
        {
            _matrix[(y * _width) + x] = 0;
        }
    }


    if(true == render)
       retVal = show();

    return retVal;
}


void Ws2811Wrapper::setClearOnExit(bool clear)
{
    _clearOnExit = clear;
}


void Ws2811Wrapper::setPixelColor(ws2811_led_t color)
{
    u_int32_t x, y;

    for (y = 0; y < _height; y++)
    {
        for (x = 0; x < _width; x++)
        {
            _matrix[ (y * _width) + x] = color;
        }
    }

}

void Ws2811Wrapper::setPixelColor(u_int32_t width, u_int32_t hight, ws2811_led_t color)
{
    _matrix[width * hight] = color;
}


void Ws2811Wrapper::setPixelColor(u_int32_t width, u_int32_t hight, u_int8_t red, u_int8_t green, u_int8_t blue)
{
    _matrix[width * hight] = Color(red, green, blue);
}

void Ws2811Wrapper::setPixelColor(u_int32_t width, u_int32_t hight, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white)
{
    _matrix[width * hight] = Color(red, green, blue, white);
}

ws2811_led_t Ws2811Wrapper::getPixelColor(u_int32_t width, u_int32_t hight)
{
    return _matrix[width * hight];
}

void Ws2811Wrapper::setBrightness(u_int8_t brightness)
{
    _ledstring.channel[0].brightness = brightness;
    _ledstring.channel[1].brightness = brightness;
}


u_int32_t Ws2811Wrapper::getNumberLeds()
{
    return _ledstring.channel[0].count;
}

u_int32_t Ws2811Wrapper::getHight()
{
    return _height;
}

u_int32_t Ws2811Wrapper::getWidth()
{
    return _width;
}

ws2811_led_t Ws2811Wrapper::Color(u_int8_t red, u_int8_t green, u_int8_t blue)
{
    return ((u_int32_t) green << 16) | ((u_int32_t) red << 8) | blue;
}

ws2811_led_t Ws2811Wrapper::Color(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white)
{
    return ((u_int32_t) white << 24) | ((u_int32_t) green << 16) | ((u_int32_t) red << 8) | blue;
}

ws2811_led_t Ws2811Wrapper::Wheel(u_int8_t wheelPos)
{
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85)
  {
    return Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }

  if(wheelPos < 170)
  {
    wheelPos -= 85;
    return Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }

  wheelPos -= 170;
  return Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

ws2811_led_t Ws2811Wrapper::Red(ws2811_led_t color)
{
    return (color >> 16) & 0xFF;
}

ws2811_led_t Ws2811Wrapper::Green(ws2811_led_t color)
{
    return (color >> 8) & 0xFF;
}

ws2811_led_t Ws2811Wrapper::Blue(ws2811_led_t color)
{
    return color & 0xFF;
}

ws2811_led_t Ws2811Wrapper::DimColor(ws2811_led_t color)
{
    ws2811_led_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

void Ws2811Wrapper::waitSec(u_int32_t sec)
{
    timespec deadline;

    deadline.tv_sec = sec;
    deadline.tv_nsec = 0;

    clock_nanosleep(CLOCK_REALTIME, 0 , &deadline, NULL);
}

void Ws2811Wrapper::waitMillSec(u_int32_t mill)
{
    timespec deadline;

    deadline.tv_sec = mill / 1000;
    deadline.tv_nsec = (mill * 1000000) % 1000000000;

    clock_nanosleep(CLOCK_REALTIME, 0 , &deadline, NULL);
}
