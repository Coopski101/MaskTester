#include "Particles.h"

Particles::Particles(){//constructor
    Particles::part03um= Particles::part05um= Particles::part10um= Particles::part25um= Particles::part50um= Particles::part100um = 0.0;
}

void Particles::readParticles(Adafruit_PM25AQI* aqiAddr){//read from sensor and store in object
    PM25_AQI_Data data;// compiler warning, but it will only make it here if there is a funtino device
    //data.particles_03um = 0;
    
    if( aqiAddr->read(&data)){//ensure reading is actually done
        Particles::part03um = data.particles_03um;
        Particles::part05um = data.particles_05um;
        Particles::part10um = data.particles_10um;
        Particles::part25um = data.particles_25um;
        Particles::part50um = data.particles_50um;
        Particles::part100um = data.particles_100um;       
    }
}
