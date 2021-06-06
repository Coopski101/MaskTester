/*By Cooper Zuranski - April 2021*/
#include "Mask.h"

#define LEDPIN 3
#define BUTTONPIN 4 
#define PERIODBLINK 2500 //ms
#define LENBLINK 300 //ms 50% duty
 
//global is good practice in main file in arduino c++
Adafruit_BME280 bmeOut;//0x76
Adafruit_BME280 bmeIn; //0x77
Adafruit_PM25AQI aqi;//0x12
File outFile;

void setup() {
    pinMode(10,OUTPUT);//chipselect 10 uno, 53 mega
    pinMode(LEDPIN,OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);//use built in led as error light
    pinMode(BUTTONPIN,INPUT_PULLUP);//startbutton input
    Wire.begin();
    //Serial.begin(115200); while(!Serial){};//debugging
    
    //initialize devices and ensure success
    delay(1000);//pm25 sensor was struggling to start up in time
    if(!SD.begin())//one blink - sd card
        errorBlink(1,LENBLINK,PERIODBLINK);
    if (!(bmeIn.begin(0x77, &Wire))) //two blinks - regular bme280
        errorBlink(2,LENBLINK,PERIODBLINK);
    if (!bmeOut.begin(0x76, &Wire)) //three blinks - Jumpered BME280
        errorBlink(3,LENBLINK,PERIODBLINK);
    if (! aqi.begin_I2C()) //4 blinks - pm25  
        errorBlink(4,LENBLINK,PERIODBLINK);
}



/////////////////////////////////////////////////////////////////////////////////"main"
void loop(){ 
    uint16_t i=0;
    
    while( digitalRead(BUTTONPIN) == LOW){
        if(++i == 65000){//how long counts as a "button press"
            outFile = newFile();
            Mask* testing = new Mask(&outFile, BUTTONPIN, LEDPIN, &aqi, &bmeIn, &bmeOut);
            delay(3000); 
            outFile.close();  
            delete testing;  
        }
    }//whilepressed
    
}//loopMAIN




//no prototypes needed
File newFile(){//creates new file name using defined naming convention based on what is on the system already
    File root = SD.open("/");
    File myFile;
    long fileNum = 0;
    while(File entry = root.openNextFile()){//seeing next allowable file number for file name 
        fileNum++;
        entry.close();
    }
    root.close();
    
    if(fileNum >= 999999){//ensure validity; bc of 8.3 exfat convention, only 8 char long name, 3 extension
        errorBlink(5,LENBLINK,PERIODBLINK);//files need to be removed from sd
    }else{
        String thisFile = "M_" + (String)fileNum + ".txt";//easier to concat with string
        char* fileName = (char*)thisFile.c_str();//convert to argument of open
        
        if(!(myFile = SD.open(fileName, FILE_WRITE)))
            errorBlink(6,LENBLINK,PERIODBLINK);  
    }//ifelse file valid
    return myFile;
}

void errorBlink(int numBlinks, int lenBlinks, int period){//stall device in error state with blinking error code
    if( (period - (numBlinks*lenBlinks)) <= 0){ //check if passed values are reasonable
        while(1){//rapidly flash so as to indicate the bug to the function user
            digitalWrite(LEDPIN, HIGH);
            delay(100);
            digitalWrite(LEDPIN, LOW);
            delay(100);
        }//while 1
    }//if erroneous fn use
    
    int remaining = period - (numBlinks*lenBlinks); 
    while(1){//also stop all program progress - but for the INTENDED use of implementation
        for(int i=numBlinks; i>0; i--){
            digitalWrite(LEDPIN, HIGH);
            delay(lenBlinks/2);
            digitalWrite(LEDPIN, LOW);
            delay(lenBlinks/2);
        }//for
        delay(remaining);
    }//while
}//errorblink

/*
void tiredOfClearingSD(){//utility function for use during development - parameters not needed, only one SD card
    File root = SD.open("/");
    while(File entry = root.openNextFile()){//find viable file name  
        SD.remove(entry.name());
        entry.close();
    }
    root.close();
}
*/
