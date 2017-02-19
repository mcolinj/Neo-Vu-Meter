/* simple moving Blip with variable speed */ 
#ifndef __VUMETER1076_H__
#define __VUMETER1076_H__

#include "Arduino.h"

/* Forward declaration to avoid dependency. */
class Adafruit_NeoPixel;
#define VUMETER_FPS                10
#define VUMETER_HANG_ITERS         10
#define VUMETER_HANG_FPS           (VUMETER_FPS * VUMETER_HANG_ITERS)

class VuMeter1076 {
    /*
     *  Predefined VU Meter colors
     */
    static const uint32_t green;
    static const uint32_t yellow;
    static const uint32_t red;
    static const uint32_t off;

    public:
        VuMeter1076(Adafruit_NeoPixel &pixels, int first_pixel, int last_pixel);
        uint16_t numPixels(void);
        void show(void);
        uint32_t pixelColor(int index);
        void setMeterPixel(int index);
        void clearMeterPixel(int index);
        void setMeterValue(int sensorValue);
        int meterValue(void);
        void hangPixels(int sensorValue);

    private:
        Adafruit_NeoPixel &_pixels;
        int _start_index;
        int _end_index;
        int _value;            // value to be displayed
        int _last_value;       // last value displayed (for erase)
        float _high_water;     // high water mark for falldown
        int _hang_time;        // 
        int _up_down;          // +1 for up,  -1 for down
        uint16_t _num_pixels;
};

#endif
