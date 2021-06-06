#include "Mask.h"
//assuming max instances of tests is 3 on UNO!

Mask::Mask(){
    Mask::outFile = NULL;
    Mask::testCount = 0;
}//mask


Mask::~Mask(){//free heap memory
    for(uint8_t i = 0; i<3; i++){
        delete Mask::testList[i];
    }
}//mask destructor


Mask::Mask(File* outFile, uint8_t buttonPin, uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){
    Mask::outFile = outFile;
    Mask::testCount = 0;
    Mask::maskMenu(buttonPin, ledPin, aqiAddr, bmeInAddr, bmeOutAddr);//go into "menu" on creation
}//all inclusive constructor called from outside


void Mask::maskMenu(uint8_t buttonPin, uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){//handles "interface" of adding a new test (startNewTest) or exitAndClose
    uint8_t choice = 3;// 1 or 2 are the options: 1 add, 2 quit
    
    do{
        delay(1000);//for after a test, needs a delay
        choice = Mask::maskChoice( buttonPin, ledPin);
        if(choice == 1){
            Mask::testList[Mask::testCount] = Mask::startNewTest(ledPin, aqiAddr, bmeInAddr, bmeOutAddr);
            Mask::testCount++;
        }
    }while( (Mask::testCount < 3 ) && (choice != 2)  ); //change limit depending on RAM capability
    
    Mask::exitAndClose();
}//mask menu


uint8_t Mask::maskChoice(uint8_t buttonPin, uint8_t ledPin){//handles the physical interface selection
    uint32_t i=0;
    uint16_t flash = 0;//should have same upper bound in rest of fn too!
    
    while(1){
        if((++flash)% 5000 == 0){//oscillitate a flashing to show a selection hasnt been made
            digitalWrite(ledPin, !digitalRead(ledPin));//toggle
            if(flash >= 65000)//reset to stay in bounds
                flash = 0;
        }//flasher
        
        while(digitalRead(buttonPin) == LOW){
            digitalWrite(ledPin,HIGH);//make led high while user is pressing
            if(++i == 65000){//how long counts as a short "button press"
                while(digitalRead(buttonPin) == LOW){
                    if(++i == 1000000)//what constitutes as a long press -> save and exit
                        return 2;
                }//while short or long  
                return 1;//short press -> start a new test
            }//if button presed long enough
        }//while button pressed
    }//while no selection made
}//mask choice


Test* Mask::startNewTest(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){//blink if not enough room
    Test* thisTest = new Test();
    digitalWrite(ledPin, LOW);
    delay(1500);//additional delay
    Mask::outFile->print(thisTest->runTest(ledPin, aqiAddr, bmeInAddr, bmeOutAddr));
    Mask::outFile->print("\n");//i wanted to be able to control the delimiter explicitly so i can parse later easily
    Mask::outFile->flush();//flush so buffer doesnt consume memory!
    return thisTest;
}



void Mask::exitAndClose(){//calls all statistic methods and creates csv for file
    //stats and csv
    for(int argInt = 0; argInt < 3; argInt++){
        Mask::outFile->print(Mask::statistics(argInt) );
        Mask::outFile->print("\n");
        Mask::outFile->flush();
    }
    //Mask::outFile->close();
    //Mask::outFile = NULL;//just cuz
}


String Mask::statistics( int arg){//y dependant is particles <= 5um, x independatnt is arg 
    float avgX, avgY, delX, delY, dx, dy, slope, yInt, numRSq, denRSq, rSq;
    avgX= avgY= dx= dy= denRSq= numRSq = 0.0;
    uint8_t i;
    String returnString = "";
    if(arg == PT)//start ouput string with specific regression variables
        returnString.concat("PT,");
    else if(arg == PH)
        returnString.concat("PH,");
    else
        returnString.concat("PP,");
        
    for(i = 0; i < Mask::testCount; i++){//FIND MEAN
        avgY += Mask::testList[i]->getInfectAeroSum();
        if(arg == PT)
            avgX += Mask::testList[i]->getTempOut();
        else if(arg == PH)
            avgX += Mask::testList[i]->getHumdOut();
        else
            avgX += Mask::testList[i]->getPresOut();
    }
    avgX /= Mask::testCount;
    avgY /= Mask::testCount;
    
    for( i = 0; i < Mask::testCount; i++){// sum dx and dy
        if(arg == PT)
            dx += pow( (delX = (Mask::testList[i]->getTempOut() - avgX)), 2); //(x - x_ )^2 
        else if(arg == PH)
            dx += pow( (delX = (Mask::testList[i]->getHumdOut() - avgX)), 2);
        else
            dx += pow( (delX = (Mask::testList[i]->getPresOut() - avgX)), 2);
        dy += delX * (delY = (Mask::testList[i]->getInfectAeroSum()- avgY));// (x-x_)*(y-y_)
        denRSq += pow(delY,2); //(y-y_)^2 -> for use with rsquared
    }
    slope = dy/dx ;
    yInt = avgY - avgX*slope;
    
    for( i = 0; i < Mask::testCount; i++){// numerator of r squared
       if(arg == PT)
            numRSq += pow( ( (yInt + slope*(Mask::testList[i]->getTempOut())) - avgY ), 2);// (yRegress - y_)^2
        else if(arg == PH)
            numRSq += pow( ( (yInt + slope*(Mask::testList[i]->getHumdOut())) - avgY ), 2);
        else
            numRSq += pow( ( (yInt + slope*(Mask::testList[i]->getPresOut())) - avgY ), 2);
       
    }
    rSq = numRSq/denRSq;

    returnString.concat(String(yInt,6)); returnString.concat(","); 
    returnString.concat(String(slope,6)); returnString.concat(",");
    returnString.concat(String(rSq,6)); 
    
    return returnString;
}
