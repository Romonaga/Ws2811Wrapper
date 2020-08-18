#include "ws2811wrapper.h"

#include <cstring>
#include <time.h>


#include <iostream>

Ws2811Wrapper::Ws2811Wrapper()
{


    std::memset(&_ledstring, '\0', sizeof(_ledstring));
    _ledstring.freq = WS2811_TARGET_FREQ;
    _ledstring.dmanum =  0;

    _ledstring.channel[Channel1].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[Channel1].gpionum = 0;
    _ledstring.channel[Channel1].count = 0;
    _ledstring.channel[Channel1].invert = 0;
    _ledstring.channel[Channel1].brightness = 127;
    _ledstring.channel[Channel1].leds = nullptr;
    _ledstring.channel[Channel1].gamma = nullptr;


    _ledstring.channel[Channel2].strip_type  = WS2811_STRIP_RGB;
    _ledstring.channel[Channel2].gpionum = 0;
    _ledstring.channel[Channel2].count = 0;
    _ledstring.channel[Channel2].invert = 0;
    _ledstring.channel[Channel2].brightness = 127;
    _ledstring.channel[Channel2].leds = nullptr;
    _ledstring.channel[Channel2].gamma = nullptr;

    _stripTypes[Channel1] = NONE;
    _stripTypes[Channel2] = NONE;

    _curChannel = Channel1;
    _clearOnExit = true;

    _matrix[Channel1] = nullptr;
    _matrix[Channel2] = nullptr;

    _rgbMatrix = nullptr;


}

Ws2811Wrapper::~Ws2811Wrapper()
{

    if(true == _clearOnExit)
        clearLeds();

    cleanUp();

}


ws2811_return_t Ws2811Wrapper::show()
{
    ws2811_return_t result = WS2811_SUCCESS;


    if(_stripTypes[_curChannel] != MATRIX_2121)
    {
        for (int led = 0; led < _ledstring.channel[_curChannel].count; led++)
        {
            _ledstring.channel[_curChannel].leds[led] = _matrix[_curChannel][led];
        }
        result = ws2811_render(&_ledstring);
    }
    else if(_stripTypes[_curChannel] == MATRIX_2121)
    {
        int led = 0;
        for (uint32_t row = 0; row < _rows[_curChannel] ; row++)
        {
            for (uint32_t col  = 0; col  < _columns[_curChannel]; col++)
            {
                led = (row * _columns[_curChannel]) + col;
                _frame->SetPixel(col, row, Red(_matrix[_curChannel][led]), Green(_matrix[_curChannel][led]), Blue(_matrix[_curChannel][led]) );
            }
        }

        _frame  = _rgbMatrix->SwapOnVSync(_frame);
    }

    return result;
}

void Ws2811Wrapper::setCustomGammaCorrection(double gammaFactor)
{
    if( (_stripTypes[Channel1] != NONE && _stripTypes[Channel2] != NONE) && (_stripTypes[Channel1] != MATRIX_2121 || _stripTypes[Channel2] != MATRIX_2121))
        ws2811_set_custom_gamma_factor(&_ledstring, gammaFactor);
    else if(_stripTypes[Channel1] == MATRIX_2121)
        _frame->set_luminance_correct((gammaFactor <= 0) ? false : true);

}

void Ws2811Wrapper::cleanUp()
{
    if(_matrix[Channel1] != nullptr)
    {
        delete [] _matrix[Channel1];
        _matrix[Channel1] = nullptr;
    }

    if(_matrix[Channel2] != nullptr)
    {
        delete [] _matrix[Channel2];
        _matrix[Channel2] = nullptr;
    }

    if(_rgbMatrix != nullptr)
    {
       _rgbMatrix->Clear();
       delete _rgbMatrix;
    }

    if( (_stripTypes[Channel1] != NONE && _stripTypes[Channel2] != NONE) && (_stripTypes[Channel1] != MATRIX_2121 || _stripTypes[Channel2] != MATRIX_2121))
        ws2811_fini(&_ledstring);

}

void Ws2811Wrapper::setCurChannel(ws2811Channel curChannel)
{
    _curChannel = curChannel;
}

ws2811Channel Ws2811Wrapper::getCurChannel() const
{
    return _curChannel;
}

ws2811_return_t Ws2811Wrapper::initStrip(ws2811Channel channel, u_int32_t rows, u_int32_t columns, LedStripType stripType, int dma, int gpio, matrixDirection matrixDir)
{
    ws2811_return_t retval = WS2811_SUCCESS;

    _curChannel = channel;
    _columns[_curChannel] = columns;
    _rows[_curChannel] = rows;
    _matrixDirection[_curChannel] = matrixDir;

    if(_ledstring.channel[_curChannel].count > 0)
        cleanUp();


    _ledstring.channel[_curChannel].count = _rows[_curChannel] * _columns[_curChannel];

    setStripType(stripType);

    switch (gpio)
    {
        case 12:
        case 18:
        case 40:
        case 52:
            _ledstring.channel[_curChannel].gpionum = gpio;
            break;

        default:
            retval = WS2811_ERROR_GPIO_INIT;
            break;
    }

    if (dma < 14)
        _ledstring.dmanum = dma;
    else
        retval = WS2811_ERROR_DMA;

    if (retval == WS2811_SUCCESS)
    {
        _matrix[_curChannel] =  new ws2811_led_t[sizeof(ws2811_led_t) * (_rows[_curChannel] * _columns[_curChannel])];
        retval =  ws2811_init(&_ledstring);
    }


    return retval;
}

static void PrintOptions(const RGBMatrix::Options &o) {
#define P_INT(val) fprintf(stderr, "%s : %d\n", #val, o.val)
#define P_STR(val) fprintf(stderr, "%s : %s\n", #val, o.val)
#define P_BOOL(val) fprintf(stderr, "%s : %s\n", #val, o.val ? "true":"false")
  P_STR(hardware_mapping);
  P_INT(rows);
  P_INT(cols);
  P_INT(chain_length);
  P_INT(parallel);
  P_INT(pwm_bits);
  P_INT(pwm_lsb_nanoseconds);
  P_INT(pwm_dither_bits);
  P_INT(brightness);
  P_INT(scan_mode);
  P_INT(row_address_type);
  P_INT(multiplexing);
  P_BOOL(disable_hardware_pulsing);
  P_BOOL(show_refresh_rate);
  P_BOOL(inverse_colors);
  P_STR(led_rgb_sequence);
  P_STR(pixel_mapper_config);
  P_STR(panel_type);
  P_INT(limit_refresh_rate_hz);
#undef P_INT
#undef P_STR
#undef P_BOOL
}

ws2811_return_t Ws2811Wrapper::initStrip(u_int32_t rows, u_int32_t columns, LedStripType stripType, matrixDirection matrixDir, Wiring2121 wiring)
{
    ws2811_return_t retval = WS2811_SUCCESS;

    _curChannel = Channel1;
    _columns[_curChannel] = columns;
    _rows[_curChannel] = rows;
    _wiring[_curChannel] = wiring;
    setStripType(stripType);

    switch(wiring)
    {
    case  Regular:
        _led_options.hardware_mapping = "regular";
        break;

    case Adafruithat:
        _led_options.hardware_mapping = "adafruit-hat";
        break;


    case Adafruithatpwm:
        _led_options.hardware_mapping = "adafruit-hat-pwm";
        break;

     default:
        _led_options.hardware_mapping = "regular";
        break;


    }

    _led_options.pwm_bits = 7;


    _led_options.rows = rows;
    _led_options.cols = columns;
    _led_options.chain_length = 1;
    _led_options.parallel = 1;
    _led_options.multiplexing = 0;
    _led_options.show_refresh_rate = false;
    _runtime.drop_privileges = -1;
    _runtime.gpio_slowdown = 5;
    //_runtime.daemon = 1;
    _runtime.do_gpio_init = true;

    PrintOptions(_led_options);
    _matrixDirection[_curChannel] = matrixDir;

    _rgbMatrix = CreateMatrixFromOptions(_led_options, _runtime);

     if(_rgbMatrix == nullptr)
          retval = WS2811_ERROR_GPIO_INIT; //not correct error but will work for now.

     if (retval == WS2811_SUCCESS)
     {
         _frame  = _rgbMatrix->CreateFrameCanvas();
         _matrix[_curChannel] =  new ws2811_led_t[sizeof(ws2811_led_t) * (_rows[_curChannel] * _columns[_curChannel])];

     }
    return retval;

}

void Ws2811Wrapper::setStripType(LedStripType stripType)
{
    _stripTypes[_curChannel] = stripType;

    switch(stripType)
    {

    case NEO_WS2811_STRIP_RGB:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_RGB;
        break;

    case NEO_WS2811_STRIP_RBG:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_RBG;
        break;

    case NEO_WS2811_STRIP_GRB:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_GRB;
        break;

    case NEO_WS2811_STRIP_GBR:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_GBR;
        break;

    case NEO_WS2811_STRIP_BRG:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_BRG;
        break;

    case NEO_WS2811_STRIP_BGR:
        _ledstring.channel[_curChannel].strip_type = WS2811_STRIP_BGR;
        break;

    case NEO_SK6812_STRIP_RGBW:
        _ledstring.channel[_curChannel].strip_type = SK6812_STRIP_RGBW;
        break;

    case NEO_SK6812_STRIP_GRBW:
        _ledstring.channel[_curChannel].strip_type = SK6812_STRIP_GRBW;
        break;

    case MATRIX_2121:
        break;

    default:
        //todo logerror
        break;
    }
}

const char * Ws2811Wrapper::getws2811ErrorString(const ws2811_return_t state)
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

    for (u_int32_t row = 0; row < _rows[_curChannel] ; row++)
    {
        for (u_int32_t col = 0; col < _columns[_curChannel]; col++)
            _matrix[_curChannel][(row * _columns[_curChannel]) + col] = 0;
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

    for (u_int32_t row = 0; row < _rows[_curChannel]; row++)
    {
        for (u_int32_t col = 0; col < _columns[_curChannel]; col++)
        {
            _matrix[_curChannel][ (row * _columns[_curChannel]) + col] = color;
        }
    }

}

u_int32_t Ws2811Wrapper::getPixelIndex(u_int32_t row, u_int32_t pixel)
{
    u_int32_t index = 0;

    switch(_matrixDirection[_curChannel])
    {
        case TopLeftRight:
            index = (row % 2 != 0) ? ((row * _columns[_curChannel]) + pixel)  : (row * _columns[_curChannel]) + ((_columns[_curChannel] - 1) - pixel);
        break;

        case TopRightDown:
            index = (pixel % 2 == 0) ? (pixel * _rows[_curChannel]) + row : (pixel * _rows[_curChannel]) + (_rows[_curChannel] - 1) - row;
            break;

        case BottomRightUp:
            index = (pixel % 2 != 0) ? (pixel * _rows[_curChannel]) + row : (pixel * _rows[_curChannel]) + (_rows[_curChannel] - 1) - row;
        break;

        case TopRightLeft:
            index = (row % 2 == 0) ? ((row * _columns[_curChannel]) + pixel)  : (row * _columns[_curChannel]) + pixel;
        break;

        case MatrixRightLeft:
             index = (row * _columns[_curChannel]) + ((_columns[_curChannel] - 1) - pixel);
        break;

        case MatrixTopDown:
             index = (row * _columns[_curChannel]) + pixel;
           //  fprintf(stderr,"row: %d col: %d index: %d\n",row, pixel, index);
        break;
    }

  //fprintf(stderr,"getPixalIndex row: %d col: %d, index: %d\n",row, pixal, index);

    return index;


}

void Ws2811Wrapper::setPixel(u_int32_t pixel, ws2811_led_t color)
{
    if( pixel > getNumberLeds() - 1)  return;

    _matrix[_curChannel][pixel] = color;

}

void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixel, ws2811_led_t color)
{
     setPixel(getPixelIndex(row, pixel), color );
}

void Ws2811Wrapper::setPixelColor(u_int32_t pixel, ws2811_led_t color)
{
    setPixel(pixel, color);
}



void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixel, u_int8_t red, u_int8_t green, u_int8_t blue)
{
     setPixel(getPixelIndex(row, pixel), Color(red, green, blue));
}

void Ws2811Wrapper::setPixelColor(u_int32_t row, u_int32_t pixel, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white)
{
    setPixel(getPixelIndex(row, pixel), Color(red, green, blue, white));
}

ws2811_led_t Ws2811Wrapper::getPixelColor(u_int32_t row, u_int32_t pixel)
{   
    if( pixel > getNumberLeds() - 1)  return 0;

    return getPixelColor(getPixelIndex(row, pixel));


}

ws2811_led_t Ws2811Wrapper::getPixelColor(u_int32_t pixel)
{
    if( pixel > getNumberLeds() - 1)  return 0;

    return _matrix[_curChannel][pixel];
}



void Ws2811Wrapper::setBrightness(u_int8_t brightness)
{
    if(_stripTypes[_curChannel] != MATRIX_2121)
    {
        _ledstring.channel[_curChannel].brightness = brightness;
    }
    else
    {
        _rgbMatrix->SetBrightness(brightness);
    }

}


u_int32_t Ws2811Wrapper::getNumberLeds()
{
    return (_stripTypes[_curChannel]  != MATRIX_2121) ? _ledstring.channel[_curChannel].count : _columns[_curChannel] * _rows[_curChannel];
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

    return ((u_int32_t) red << 16) | ((u_int32_t) green << 8) | blue;


}

ws2811_led_t Ws2811Wrapper::Color(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t white)
{
    return ((u_int32_t) white << 24) | ((u_int32_t) red << 16) | ((u_int32_t) green << 8) | blue;

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

const char * getws2811ErrorString(const ws2811_return_t state)
{
    return ws2811_get_return_t_str(state);
}
