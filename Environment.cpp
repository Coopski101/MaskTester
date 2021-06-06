#include "Environment.h"

Environment::Environment(){
    Environment::tempOut = Environment::humdOut = Environment::presOut = Environment::tempIn = Environment::humdIn = Environment::presIn = 0.0;
}

void Environment::readEnvironment(Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){//ptr to each sensor, take all readings, store 
    Environment::tempOut = bmeOutAddr->readTemperature();//fail to read should still return a valid value, no catch needed
    Environment::humdOut = bmeOutAddr->readHumidity();
    Environment::presOut = bmeOutAddr->readPressure();
    Environment::tempIn = bmeInAddr->readTemperature();
    Environment::humdIn = bmeInAddr->readHumidity();
    Environment::presIn = bmeInAddr->readPressure();
}
