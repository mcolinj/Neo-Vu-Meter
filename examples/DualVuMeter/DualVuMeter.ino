#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h>
#endif
#include <VuMeter1076.h>

// How many NeoPixels are attached to the Arduino?
#define PIN_PIXELS         6
#define FPS               10

#define NUM_PIXELS         32
#define NUM_METERS          2


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS*NUM_METERS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

//
//  Create one down and one up VuMeters (back to back)
//
VuMeter1076 meter0 = VuMeter1076(pixels, 30, 0);
VuMeter1076 meter1 = VuMeter1076(pixels, 31, 61);

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
    int sensorValue = analogRead(A0) / 1.8;
    
    meter0.setMeterValue(sensorValue);
    meter1.setMeterValue(sensorValue);
    meter0.show();

    //
    // And now do a little dance to simulate
    // decay of the sticking threshold value.
    //
    for (int i = 0; i < VUMETER_HANG_ITERS; i++) {
        meter0.hangPixels(sensorValue);
        meter1.hangPixels(sensorValue);
        meter0.show();
        delay((1000 / VUMETER_HANG_FPS) - 5);
    }
}
