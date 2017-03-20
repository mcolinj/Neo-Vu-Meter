#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h>
#endif
#include <VuMeter1076.h>

// How many NeoPixels are attached to the Arduino?
#define PIN_PIXELS         6

#define NUM_PIXELS         212
#define NUM_METERS           7


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

//
//  Create one down and one up VuMeters (back to back)
//
VuMeter1076 meters[NUM_METERS] =  {
    VuMeter1076(pixels, 0, 17),
    VuMeter1076(pixels, 52, 35),
    VuMeter1076(pixels, 64, 81),
    VuMeter1076(pixels, 117, 100),
    VuMeter1076(pixels, 130, 147),
    VuMeter1076(pixels, 182, 165),
    VuMeter1076(pixels, 195, 212) } ;


void setup() {
    Serial.begin(115200);
}

//
//
//
int whichMeter = 0;

void loop() {
    /* Note this sensor value ought to be scaled
     * to not overflow the meter value.
     * Is that the responsibility of the meter?
     * For now, we get interesting effects if we
     * just let it overflow.
     */
    int sensorValue = analogRead(A0) / 1.8;
    
    meters[whichMeter].setMeterValue(sensorValue);
    meters[whichMeter].show();
    
    //
    // And now do a little dance to simulate
    // decay of the sticking threshold value.
    //
    for (int i = 0; i < NUM_METERS; i++) {
        meters[i].hangPixels(sensorValue);
        meters[i].show();
        delay((1000 / VUMETER_HANG_FPS) - 5);
    }

    whichMeter = (whichMeter + 1) % (NUM_METERS-1);

}
