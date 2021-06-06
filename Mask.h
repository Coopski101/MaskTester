#ifndef MASK_H_EXISTS
#define MASK_H_EXISTS

#include "Test.h" //wire,spi,adasenseor,arduino,pm25aqi,bme280 already included via this alone
#include <SD.h>

typedef enum args {PT, PH, PP} args;//for linear regression: particles vs (temp,humid,pressure)

class Mask{// instance is showing to be 15 bytes on the heap in tests. plus needed space for csv creation -> ea test is 51 (w/ 80bytes at peak)
    private:
        File* outFile;
        Test* testList[3];//how many this device can handle without overflow?? ea test is 51 (w/ 80bytes at peak)
        uint8_t testCount;//through testing the limit is only 3 with the UNO

        //could I have stored all these values, yes, but i'm trying to conserve memory
        void maskMenu(uint8_t buttonPin, uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);//handles "interface" of adding a new test (startNewTest) or exitAndClose
        uint8_t maskChoice(uint8_t buttonPin, uint8_t ledPin);
        Test* startNewTest(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);//blink if full, exit
        void exitAndClose();//calls all statistic methods and creates csv for file
        
        String statistics(int arg);//linear regression, rsquared based on whicheve comparison of args is passed
    public:
        Mask();//not used
        ~Mask();
        Mask(File* outFile,uint8_t buttonPin, uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr);
};//Mask

#endif
