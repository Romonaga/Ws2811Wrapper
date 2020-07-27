#include "ws2811wrapper.h"

#include <cstring>
#include <time.h>


Ws2811Wrapper::Ws2811Wrapper()
{


    std::memset(&_ledstring, '\0', sizeof(_ledstring));
    _ledstring.freq = WS2811_TARGET_FREQ;
    _ledstring.dmanum =  0;

    _ledstring.channel[0].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[0].gpionum = 0;
    _ledstring.channel[0].count = 0;
    _ledstring.channel[0].invert = 0;
    _ledstring.channel[0].brightness = 127;
    _ledstring.channel[0].leds = nullptr;
    _ledstring.channel[0].gamma = nullptr;


    _ledstring.channel[1].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[1].gpionum = 0;
    _ledstring.channel[1].count = 0;
    _ledstring.channel[1].invert = 0;
    _ledstring.channel[1].brightness = 127;
    _ledstring.channel[1].leds = nullptr;
    _ledstring.channel[1].gamma = nullptr;

    _curChannel = 0;
    _clearOnExit = true;

    _matrix[0] = nullptr;
    _matrix[1] = nullptr;
}

Ws2811Wrapper::~Ws2811Wrapper()
{

    if(true == _clearOnExit)
        clearLeds();

    cleanUp();

}

void Ws2811Wrapper::matrixraise(void)
{
    u_int32_t x, y;

    for (y = 0; y < (_columns[_curChannel] - 1); y++)
    {
        for (x = 0; x < _rows[_curChannel]; x++)
        {
            _matrix[_curChannel][y * _rows[_curChannel] + x] = _matrix[_curChannel][(y + 1)*_rows[_curChannel] + _rows[_curChannel] - x - 1];
        }
    }
}


ws2811_return_t Ws2811Wrapper::show()
{
    ws2811_return_t result = WS2811_SUCCESS;

    int x;

    for (x = 0; x < _ledstring.channel[_curChannel].count; x++)
    {
        _ledstring.channel[_curChannel].leds[x] = _matrix[_curChannel][ x];
    }

    result = ws2811_render(&_ledstring);

    return result;
}

void Ws2811Wrapper::setCustomGammaCorrection(double gammaFactor)
{
   ws2811_set_custom_gamma_factor(&_ledstring, gammaFactor);
}

void Ws2811Wrapper::cleanUp()
{
    if(_matrix[0] != nullptr)
    {
        delete [] _matrix[0];
        _matrix[0] = nullptr;
    }

    if(_matrix[1] != nullptr)
    {
        delete [] _matrix[1];
        _matrix[1] = nullptr;
    }

    ws2811_fini(&_ledstring);

}

void Ws2811Wrapper::setCurChannel(short curChannel)
{
    _curChannel = curChannel;
}

short Ws2811Wrapper::getCurChannel() const
{
    return _curChannel;
}

ws2811_return_t Ws2811Wrapper::initStrip(short channel, u_int32_t rows, u_int32_t columns, LedStripType stripType, int dma, int gpio)
{
    ws2811_return_t retval = WS2811_SUCCESS;

    if(_ledstring.channel[channel].count > 0)
        cleanUp();

    _curChannel = channel;
    _columns[_curChannel] = columns;
    _rows[_curChannel] = rows;

    _ledstring.channel[channel].count = _rows[_curChannel] * _columns[_curChannel];

    setStripType(stripType, channel);

    switch (gpio)
    {
        case 12:
        case 18:
        case 40:
        case 52:
            _ledstring.channel[channel].gpionum = gpio;
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
        _matrix[_curChannel] =  new ws2811_led_t[sizeof(ws2811_led_t) * _rows[_curChannel] * _columns[_curChannel]];
        retval =  ws2811_init(&_ledstring);
    }


    return retval;
}

void Ws2811Wrapper::setStripType(LedStripType stripType, short channel)
{
    switch(stripType)
    {

    case NEO_WS2811_STRIP_RGB:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_RGB;
        break;

    case NEO_WS2811_STRIP_RBG:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_RBG;
        break;

    case NEO_WS2811_STRIP_GRB:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_GRB;
        break;

    case NEO_WS2811_STRIP_GBR:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_GBR;
        break;

    case NEO_WS2811_STRIP_BRG:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_BRG;
        break;

    case NEO_WS2811_STRIP_BGR:
        _ledstring.channel[channel].strip_type = WS2811_STRIP_BGR;
        break;

    case NEO_SK6812_STRIP_RGBW:
        _ledstring.channel[channel].strip_type = SK6812_STRIP_RGBW;
        break;

    case NEO_SK6812_STRIP_GRBW:
        _ledstring.channel[channel].strip_type = SK6812_STRIP_GRBW;
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

    for (y = 0; y < _rows[_curChannel] ; y++)
    {
        for (x = 0; x < _columns[_curChannel]; x++)
        {
            _matrix[_curChannel][(y * _columns[_curChannel]) + x] = 0;
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

    for (y = 0; y < _rows[_curChannel]; y++)
    {
        for (x = 0; x < _columns[_curChannel]; x++)
        {
            _matrix[_curChannel][ (y * _columns[_curChannel]) + x] = color;
        }
    }

}

u_int32_t Ws2811Wrapper::getPixelIndex(u_int32_t row, u_int32_t pixal)
{
    return (row % 2 != 0) ? ((row * _columns[_curChannel]) + pixal)  : (((row * _columns[_curChannel])) + ((_columns[_curChannel] - 1) - pixal));
}


void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixal, ws2811_led_t color)
{
   _matrix[_curChannel][getPixelIndex(row, pixal)] = color;

}

void Ws2811Wrapper::setPixelColor(u_int32_t pixal, ws2811_led_t color)
{
    _matrix[_curChannel][pixal] = color;
}



void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixal, u_int8_t red, u_int8_t green, u_int8_t blue)
{

   _matrix[_curChannel][getPixelIndex(row, pixal)] = Color(red, green, blue);
}

void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixal, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white)
{
    _matrix[_curChannel][getPixelIndex(row, pixal)] = Color(red, green, blue, white);
}

ws2811_led_t Ws2811Wrapper::getPixelColor(u_int32_t row, u_int32_t pixal)
{
    return _matrix[_curChannel][getPixelIndex(row, pixal)];
}

ws2811_led_t Ws2811Wrapper::getPixelColor(u_int32_t pixal)
{
    return _matrix[_curChannel][pixal];
}



void Ws2811Wrapper::setBrightness(u_int8_t brightness)
{
    _ledstring.channel[_curChannel].brightness = brightness;

}


u_int32_t Ws2811Wrapper::getNumberLeds()
{

    return _ledstring.channel[_curChannel].count;
}

u_int32_t Ws2811Wrapper::getColumns()
{
    return _columns[_curChannel];
}

u_int32_t Ws2811Wrapper::getRows()
{
    return _rows[_curChannel];
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

int Ws2811Wrapper::Red(ws2811_led_t color)
{
    return (color >> 16) & 0xFF;
}

int Ws2811Wrapper::Green(ws2811_led_t color)
{
    return (color >> 8) & 0xFF;
}

int Ws2811Wrapper::Blue(ws2811_led_t color)
{
    return color & 0xFF;
}

ws2811_led_t Ws2811Wrapper::DimColor(ws2811_led_t color)
{
    ws2811_led_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}


ws2811_led_t Ws2811Wrapper::BrightenColor(ws2811_led_t color)
{
    ws2811_led_t dimColor = Color(Red(color) << 1, Green(color) << 1, Blue(color) << 1);
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
