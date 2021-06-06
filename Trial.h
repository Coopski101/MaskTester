#ifndef TRIAL_H_EXISTS
#define TRIAL_H_EXISTS

#include "Environment.h"
#include "Particles.h"

class Trial{//each instance is showing to be 24 bytes on the heap in tests  + 
    private:
        void trialCountdown(uint8_t ledPin);
    public:
        Particles* particles;
        Environment* environment;
        
        Trial();//no param constructor -> https://forum.arduino.cc/t/class-constructors-with-parameters/45935/6
        ~Trial();
        void runTrial(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);
        
};//trial

#endif
