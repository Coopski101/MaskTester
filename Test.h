#ifndef TEST_H_EXISTS
#define TEST_H_EXISTS

#include "Trial.h"

class Test{//each instance is showing to be 51 bytes on the heap in tests. at peak needs( 24 trial + 28*2 part/env)
    private:
        //the "avg" data members are summed, and only divided at the end. 
        //This is so not all 5 trial instances need to be in memory simeltaeously
        float avgPart03um, avgPart05um, avgPart10um, avgPart25um, avgPart50um, avgPart100um, avgTempOut, avgTempIn, avgPresOut, avgPresIn,avgHumdIn,avgHumdOut;
        uint8_t trialNum;
        Trial* currentTrial;

        void avgTrials();
        float deltaPres();//only call afer average created
    public:
        Test();
        ~Test();//so that each can be deleted as soon as it is written
        String runTest(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);
        String csvTest();
        uint32_t getInfectAeroSum();//where infectious areosols <=5um typically - this is their sum
        float getTempOut();
        float getHumdOut();
        float getPresOut();
};//Test

#endif
