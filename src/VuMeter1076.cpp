#include "Arduino.h"
#include "AdaFruit_NeoPixel.h"
#include "VuMeter1076.h"

/* VuMeter 
 *
 */
#define INITIAL_HANG_TIME  0.1
#define HANG_SPEED         2.0

#define signum(x) ((x < 0) ? -1 : (x > 0))

/*
 *  Create the VuMeter element, attached to a NeoPixel string.
 *  VuMeter claims some subset of the pixels on a string.
 *  This permits a collection of meters to be projected on
 *  a single string.
 */
VuMeter1076::VuMeter1076(Adafruit_NeoPixel &pixels,
                         int start_index, int end_index) :
       _pixels(pixels), _start_index(start_index), _end_index(end_index),
       _value(0), _last_value(0) {
    _num_pixels = abs(_start_index - _end_index)+1;
    // save state about whether meter lights move up(+1) or down(-1)
    _up_down = signum(_end_index - _start_index);
    _pixels.begin();    // fire up the NeoPixel string
}

/*
 *  How many pixels on this VuMeter
 */
uint16_t VuMeter1076::numPixels(void) {
    return _num_pixels;
}

void VuMeter1076::show(void) {
    _pixels.show();
}

/*
 *   Map the position in the meter display
 *   to the appropriate color.   Position is
 *   normalized to the 0 based position index.
 */
uint32_t VuMeter1076::pixelColor(int index) {
    if (index >= _num_pixels * 0.80) {
        return red;
    } else if (index >= _num_pixels * 0.5) {
        return yellow;
    } else {
        return green;
    }
}

/*
 *  Convert the meter local index value to the
 *  NeoPixel string index and set or clear the
 *  appropriate color in the correct location
 *  on the string.
 *
 *  Note pixelColor() is a function of local index.
 */
void VuMeter1076::setMeterPixel(int index) {
    _pixels.setPixelColor(_start_index+_up_down*index, pixelColor(index));
}

void VuMeter1076::clearMeterPixel(int index) {
    _pixels.setPixelColor(_start_index+_up_down*index, off);
}


/*
 *   nextValue should have been scaled in the 0-num_pixels
 *   range by the calling client.    All of the manipulation
 *   and the calculations in the model uses the 0 based range.
 *
 *   If the nextValue extends beyond the local VuMeter (and
 *   despoils some adjacent pixels) maybe it will look interesting.
 */
void VuMeter1076::setMeterValue(int nextValue) {

    /*
     *  For an increase, color more pixels.
     */
    if (nextValue >= _num_pixels) {
        nextValue = _num_pixels-1;
    }
    
    if (nextValue >= _last_value) {
        for (int i=_last_value; i<=nextValue; i++) {
            setMeterPixel(i);
        }
    /*
     *  For a decrease, erase only the relevant pixels.
     */
    } else if (nextValue < _last_value) {
        for (int i=_last_value; i>nextValue; i--) {
            clearMeterPixel(i);
        }
    }
    _last_value = nextValue;
}

/*
 *   Get the value currently displayed by the meter.  (why do I call it
 *   _last_value?)
 */
int VuMeter1076::meterValue(void) {
    return _last_value;
}


/*
 *   Figure out the high water mark value and set it up
 *   to decay back down to the ambient data level.
 *
 */
void VuMeter1076::hangPixels(int sensorValue) {

    if (sensorValue >= _high_water) {
        _hang_time = INITIAL_HANG_TIME * VUMETER_HANG_FPS;
        _high_water = min(sensorValue, _num_pixels-1);
    } else {
        if (_hang_time-- <= 0) {
            clearMeterPixel(_high_water);
            _high_water += _hang_time * HANG_SPEED / VUMETER_HANG_FPS;
        }
        setMeterPixel(_high_water);
    }
}

/*
 *  These are the colors for the segments of the VU Meter.
 *  Probably need an API to permit the user to customize these.
 */
const uint32_t VuMeter1076::green = Adafruit_NeoPixel::Color(0, 100, 0);  //25600
const uint32_t VuMeter1076::yellow = Adafruit_NeoPixel::Color(90, 60, 0); //5913600
const uint32_t VuMeter1076::red = Adafruit_NeoPixel::Color(100, 0, 0);    //6553600
const uint32_t VuMeter1076::off = Adafruit_NeoPixel::Color(0, 0, 0); 
