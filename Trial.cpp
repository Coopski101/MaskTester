#include "Trial.h"

Trial::Trial(){
    Trial::particles = new Particles();
    Trial::environment = new Environment();
}

Trial::~Trial(){//destructor - not much ram, need to immediately free each block of memory for the next
    delete Trial::particles;
    delete Trial::environment;
}
               
void Trial::runTrial(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){
    Trial::trialCountdown(ledPin);
    Trial::particles->readParticles(aqiAddr);
    Trial::environment->readEnvironment(bmeInAddr, bmeOutAddr);
    delay(200);
    digitalWrite(ledPin, LOW); //turn light back off
}
          
void Trial::trialCountdown(uint8_t ledPin){//start sequence for about to start a test
    for(uint8_t i=3; i>0; i--){//3,2,1 - 0 on collection handled in other method
        digitalWrite(ledPin, HIGH);
        delay(400);
        digitalWrite(ledPin, LOW);
        delay(600); 
    }//for3,2,1
        digitalWrite(ledPin, HIGH); //GO!!! take sample on 0
}

/*https://arduino.stackexchange.com/questions/77061/why-is-it-considered-bad-practice-to-use-the-new-keyword-in-arduino */
