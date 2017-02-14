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
//  Here's 1000 points recorded from a musical audio source
//  Hollyn
//  Alone (feat. Tru)
//
#define NUM_SOUND_SAMPLES 1000
uint8_t readings[NUM_SOUND_SAMPLES] = {
8,13,30,23,17,10,9,29,26,20,14,12,22,17,11,7,20,21,16,12,10,21,16,12,8,36,32,19,12,8,21,19,12,9,9,22,21,12,9,26,15,10,9,8,12,9,11,8,9,11,
10,7,8,14,11,9,8,9,13,9,11,9,29,20,14,9,9,10,9,9,8,8,12,11,9,9,42,30,12,12,15,13,14,8,10,9,10,9,9,7,14,14,8,8,12,13,11,12,10,8,
30,14,10,15,10,12,10,8,7,12,9,8,11,31,37,13,11,8,12,11,8,9,9,11,9,8,8,10,8,8,8,8,10,9,8,8,8,32,17,10,9,10,14,9,8,8,11,9,10,8,
9,44,25,13,10,11,28,32,16,13,15,24,16,9,9,28,17,11,10,12,25,16,13,9,60,26,20,10,9,17,23,10,9,20,18,12,9,29,47,17,12,10,10,11,10,9,8,11,9,8,
7,15,12,12,8,8,12,8,8,7,8,36,17,12,10,12,13,11,45,29,15,11,14,9,46,26,15,11,12,28,24,13,8,8,16,11,10,12,15,11,11,12,8,13,11,9,8,23,29,17,
9,13,14,12,10,8,9,14,14,9,8,28,31,15,12,11,32,26,18,11,9,16,17,11,11,15,11,12,9,9,12,14,8,7,11,33,19,11,9,15,14,10,9,9,11,10,10,8,21,53,
36,20,10,40,33,27,14,9,13,13,14,9,8,23,17,12,9,13,22,22,10,8,32,27,18,11,17,25,22,10,11,13,18,13,10,29,43,19,13,8,37,28,12,9,10,16,10,11,8,29,
25,11,9,9,15,13,10,11,10,39,27,58,26,17,12,14,32,19,13,19,11,11,60,50,20,21,22,58,27,22,23,22,20,12,12,9,8,10,13,9,9,39,26,15,10,17,59,22,16,11,
16,10,11,9,14,9,8,9,10,29,40,21,15,17,63,43,20,15,17,24,12,9,11,8,7,8,11,10,51,29,15,15,13,39,25,28,18,21,10,10,78,37,26,13,16,19,12,38,25,14,
17,58,40,19,18,18,14,10,10,8,9,11,8,9,8,46,27,13,11,11,72,28,18,9,12,12,11,14,21,23,18,16,11,16,39,22,15,12,67,30,19,11,12,23,20,16,10,29,17,13,
20,10,45,30,22,15,10,36,26,22,26,25,12,10,22,24,14,17,24,11,32,58,24,27,26,78,36,22,11,25,17,11,9,12,9,8,11,9,9,59,33,17,21,18,53,23,21,22,17,11,
12,10,9,9,9,9,8,9,32,20,17,20,77,38,16,17,22,17,12,10,9,8,9,8,9,10,56,30,17,15,12,37,25,16,18,18,12,9,29,49,24,17,23,19,19,62,23,16,18,85,
41,25,24,14,11,11,12,10,9,12,8,8,8,46,37,23,15,14,40,18,16,12,10,12,8,12,30,24,20,14,23,30,41,23,20,21,70,39,20,22,16,12,14,17,20,14,24,19,13,10,
9,8,7,8,7,8,9,9,10,11,9,9,9,9,11,10,9,10,12,36,24,11,12,13,13,12,14,11,13,12,14,11,12,82,30,17,18,18,16,13,15,13,38,26,12,12,16,19,19,14,
49,25,22,14,10,16,41,27,12,9,12,13,11,11,12,9,9,9,12,9,29,14,10,12,9,10,9,10,9,10,9,10,9,9,8,10,12,66,27,18,15,16,15,27,16,9,10,7,43,23,
11,13,13,13,12,10,13,48,22,15,15,18,22,21,16,13,45,31,16,16,21,17,15,19,34,52,28,12,13,12,38,17,12,9,12,27,13,12,11,10,10,10,11,10,10,10,8,10,10,8,
8,8,8,7,8,8,8,8,7,7,8,7,8,9,7,9,9,8,40,27,13,12,12,16,15,16,12,13,12,11,10,10,44,27,17,14,19,15,19,18,15,28,24,19,17,20,20,14,56,25,
37,23,14,11,11,18,15,15,12,11,10,11,9,8,10,9,9,10,12,19,11,15,14,12,11,14,13,14,24,12,10,13,15,11,10,12,28,19,26,26,14,19,38,27,18,20,21,21,14,14,
18,13,15,14,14,13,31,23,14,15,17,18,18,15,14,40,21,16,14,14,15,20,22,38,39,25,14,11,15,26,16,18,16,38,27,16,11,10,11,12,9,12,15,8,8,8,8,7,8,8,
8,8,8,8,8,9,8,8,8,8,8,8,9,9,9,8,38,23,19,12,14,22,18,15,11,10,20,21,13,11,40,23,13,14,19,27,22,13,18,41,26,15,17,17,17,16,19,43,54
};

int reading = 0;

void loop() {
    /*
     *  For this example, we don't have an audio source,
     *  so we just use a data above as the audio source.
     *  Just wrap around when the data runs out.
     */
    int sensorValue = readings[reading];
    reading++;
    if (reading >= NUM_SOUND_SAMPLES) {
        reading = 0;
    }

    //
    // Arbitrary scaling here.    Could play with this.
    //
    sensorValue /= 4.2;
    
    //
    //  Display the same value on each of the meters
    //  Maybe we could figure out how to have rolling
    //  updates to each meter...
    //
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