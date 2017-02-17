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

#define DB_TABLE_SIZE 101
double db_table[DB_TABLE_SIZE];

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

    db_table[0] = 0;
    int i;
    double n;
    for (i=1, n=1.0 ; i<DB_TABLE_SIZE ; i++, n+=1.0) {
        db_table[i] = 9*log10(n);
        Serial.print("db_table[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(db_table[i]);
    }
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
    
    
    for (int i=0; i<NUM_METERS; i++) {
        int sensorValue = analogRead(A0) / 4.3;
        meters[i].setMeterValue(sensorValue);
        meters[0].show();
    }
    

    //
    // And now do a little dance to simulate
    // decay of the sticking threshold value.
    //
    for (int i = 0; i < 2; i++) {
        for (int i=0; i<NUM_METERS; i++) {
            meters[i].hangPixels(meters[i].meterValue());
        }
        meters[0].show();
        delay((1000 / VUMETER_HANG_FPS) - 5);
    }
}
