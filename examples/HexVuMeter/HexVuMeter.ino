#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h>
#endif
#include <VuMeter1076.h>
#include <Math.h>

// Which pin is wired to the NeoPixels
#define PIN_PIXELS          6

// How many NeoPixels are attached to the Arduino?
#define NUM_PIXELS         18
#define NUM_METERS          7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(212, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

//
//  Create one down and one up VuMeters (back to back)
//
VuMeter1076 meters[NUM_METERS] =  {
    VuMeter1076(pixels, 0, 37),
    VuMeter1076(pixels, 75, 38),
    VuMeter1076(pixels, 100, 81),
    VuMeter1076(pixels, 117, 100),
    VuMeter1076(pixels, 130, 147),
    VuMeter1076(pixels, 182, 165),
    VuMeter1076(pixels, 195, 212) } ;

void setup() {
    Serial.begin(115200);
}


/*
 *  Compute log10, but protect against 0 value.
 *  We could maybe even fudge things a little bit
 *  more here if it gave us something that looked
 *  good.
 */
double safeLog10(int sensorValue) {
    if (sensorValue < 10) {
        return 1;
    } else {
        return log10(sensorValue);
    }
}

#define MIN_SCALE_VALUE 50             // never scale below this
int maxValueSoFar = MIN_SCALE_VALUE;   // nominal scaling
#define DECAY_EVERY_NTH 50
uint32_t callCount = 0;

/*
 *   This returns value strictly in the desired range.
 *   It tries to adjust the sensitivity.
 */
int scaledSensorValue(uint32_t sensorValue) {

    double correctionFactor;
    uint32_t logSensorValue;
    uint32_t scaledSensorValue;

    /* re-calibrate with a new ceiling */
    if (sensorValue > maxValueSoFar) {
        maxValueSoFar = sensorValue * 0.9;
        Serial.print("increase maxValueSoFar =");
        Serial.println(maxValueSoFar);
        callCount = 0;   /* reset the decay any time there is an increase */
    }

    if (callCount == DECAY_EVERY_NTH) {
        callCount = 0;
        maxValueSoFar *= 0.90;
        if (maxValueSoFar < MIN_SCALE_VALUE) {
            maxValueSoFar = MIN_SCALE_VALUE;
        }
        Serial.print("decay maxValueSoFar =");
        Serial.println(maxValueSoFar);
    }

    callCount++;


    /*
     * 53 because of 53 - 13 is 40 pixels?  (0-38 for robot lights)
     * Shift down by 13 to reduce number of lights in low range.
     */ 
    correctionFactor = 53/safeLog10(maxValueSoFar);
    logSensorValue = safeLog10(sensorValue);
    scaledSensorValue = logSensorValue * correctionFactor - 13;
    if (scaledSensorValue < 0) {
        scaledSensorValue = 0;
    }
    return scaledSensorValue;
}


void loop() {
    /*
     * Distribute the next data samples over all of the meters
     */
    for (int i=0; i<NUM_METERS; i++) {    
        meters[i].setMeterValue(scaledSensorValue(analogRead(A0)));
        meters[0].show();
    }

    //
    // And now do a little dance to simulate
    // decay of the sticking threshold value.
    //
    for (int j = 0; j < 2; j++) {
        for (int i=0; i<NUM_METERS; i++) {
            meters[i].hangPixels(meters[i].meterValue());
        }
        meters[0].show();
        delay((1000 / VUMETER_HANG_FPS) - 5);
    }
}
