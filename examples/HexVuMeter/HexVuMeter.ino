#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h>
#endif
#include <VuMeter1076.h>

// Which pin is wired to the NeoPixels
#define PIN_PIXELS          6

// How many NeoPixels are attached to the Arduino?
#define NUM_PIXELS         10
#define NUM_METERS          6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS*NUM_METERS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

//
//  Create one down and one up VuMeters (back to back)
//
VuMeter1076 meters[NUM_METERS] =  {
    VuMeter1076(pixels, 0, 9),
    VuMeter1076(pixels, 19, 10),
    VuMeter1076(pixels, 20, 29),
    VuMeter1076(pixels, 39, 30),
    VuMeter1076(pixels, 40, 49),
    VuMeter1076(pixels, 59, 50) } ;


void setup() {
    Serial.begin(115200);
}

//
//
//
void loop() {
    /* Note this sensor value ought to be scaled
     * to not overflow the meter value.
     * Is that the responsibility of the meter?
     * For now, we get interesting effects if we
     * just let it overflow.
     */
    int sensorValue = analogRead(A0) / 4.2;
    
    for (int i=0; i<NUM_METERS; i++) {
        meters[i].setMeterValue(sensorValue);
    }
    meters[0].show();

    //
    // And now do a little dance to simulate
    // decay of the sticking threshold value.
    //
    for (int i = 0; i < VUMETER_HANG_ITERS; i++) {
        for (int i=0; i<NUM_METERS; i++) {
            meters[i].hangPixels(sensorValue);
        }
        meters[0].show();
        delay((1000 / VUMETER_HANG_FPS) - 5);
    }
}
