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
    VuMeter1076(pixels, 0, 17),
    VuMeter1076(pixels, 52, 35),
    VuMeter1076(pixels, 64, 81),
    VuMeter1076(pixels, 117, 100),
    VuMeter1076(pixels, 130, 147),
    VuMeter1076(pixels, 182, 165),
    VuMeter1076(pixels, 195, 212) } ;

#define DB_TABLE_SIZE 1
uint8_t db_table[DB_TABLE_SIZE];

double sensorValueToDecibels(int sensorValue) {
    if (sensorValue >= DB_TABLE_SIZE) {
        sensorValue = DB_TABLE_SIZE-1;
    }
    if (sensorValue < 0) {
        sensorValue = 0;
    }
    return db_table[sensorValue];
}

void setup() {
    Serial.begin(115200);
}

/*
 *
 */
double sensorValueToLog(int sensorValue) {
    if (sensorValue == 0) {
        return 0;
    } else if (sensorValue == 1) {
        return 0;
    } else {
        return log10(sensorValue);
    }
}

//
//
int maxValueSoFar = 0;
#define DECAY_EVERY_NTH 5
int loopCount = 0;
double correction = 0;
//
void loop() {

    loopCount++;
    if ((loopCount % 100) == 0) {
        maxValueSoFar *= 0.90;
        Serial.print("decay maxValueSoFar =");
        Serial.println(maxValueSoFar);
        correction = 22/sensorValueToLog(maxValueSoFar);
        Serial.print("correction = ");
        Serial.println(correction);
    }

    /*
     * Note this sensor value ought to be scaled
     * to not overflow the meter value.
     * Is that the responsibility of the meter?
     * For now, we get interesting effects if we
     * just let it overflow.
     */

    
    for (int i=0; i<NUM_METERS; i++) {
        int sensorValue = analogRead(A0);
        if (sensorValue > maxValueSoFar) {
            maxValueSoFar = sensorValue;
            Serial.print("increase maxValueSoFar =");
            Serial.println(maxValueSoFar);
        }
        correction = 22/sensorValueToLog(maxValueSoFar);  
        Serial.print("correction = ");
        Serial.println(correction);
        meters[i].setMeterValue(correction*sensorValueToLog(sensorValue)-correction);
        Serial.print("meter value =");
        Serial.println(meters[i].meterValue());
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
