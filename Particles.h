#ifndef PARTICLES_H_EXISTS
#define PARTICLES_H_EXISTS

#include <Adafruit_PM25AQI.h>

class Particles{//each instance is showing to be 28 bytes on the heap in tests
    public:
        float part03um, part05um, part10um, part25um, part50um, part100um;//0.5, 2.5 ect  
    
        Particles();//no param constructor -> https://forum.arduino.cc/t/class-constructors-with-parameters/45935/6
        void readParticles(Adafruit_PM25AQI* aqiAddr);
};//particles

#endif
