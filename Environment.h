#ifndef ENVIRONMENT_H_EXISTS
#define ENVIRONMENT_H_EXISTS

#include <Adafruit_BME280.h>//wire,spi,adasensor,arduino all included by default

class Environment{//each instance is showing to be 28 bytes on the heap in tests
    public:
        float tempOut, humdOut, presOut, tempIn, humdIn, presIn;

        //no destructor bc it never needs to be deleted on its own; always in parent
        Environment();//no param constructor -> https://forum.arduino.cc/t/class-constructors-with-parameters/45935/6
        void readEnvironment(Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);
};//environment

#endif
