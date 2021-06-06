#include "Test.h"

Test::Test(){
    Test::avgPart03um= Test::Test::avgPart05um= Test::Test::avgPart10um= Test::Test::avgPart25um= Test::avgPart50um= 
      Test::avgPart100um= Test::avgTempOut= Test::avgTempIn= Test::avgPresOut= Test::avgPresIn= Test::avgHumdIn= 
        Test::avgHumdOut =0.0;
    Test::trialNum = 0;
    Test::currentTrial= NULL;
}

Test::~Test(){//so that each can be deleted as soon as it is written
    delete Test::currentTrial;
}

String Test::runTest(uint8_t ledPin, Adafruit_PM25AQI* aqiAddr, Adafruit_BME280 *bmeInAddr, Adafruit_BME280 *bmeOutAddr){
    for(Test::trialNum=0; Test::trialNum<5; Test::trialNum++){// do five trials
        
        Test::currentTrial = new Trial();
        delay(2000);//wait 2 seconds between coughs
        Test::currentTrial->runTrial( ledPin,  aqiAddr,  bmeInAddr, bmeOutAddr);
        
        Test::avgPart03um += Test::currentTrial->particles->part03um;//sum
        Test::avgPart05um += Test::currentTrial->particles->part05um;
        Test::avgPart10um += Test::currentTrial->particles->part10um;
        Test::avgPart25um += Test::currentTrial->particles->part25um;
        Test::avgPart50um += Test::currentTrial->particles->part50um;
        Test::avgPart100um += Test::currentTrial->particles->part100um;
        Test::avgTempOut += Test::currentTrial->environment->tempOut;
        Test::avgTempIn += Test::currentTrial->environment->tempIn;
        Test::avgPresOut += Test::currentTrial->environment->presOut;
        Test::avgPresIn += Test::currentTrial->environment->presIn;
        Test::avgHumdOut += Test::currentTrial->environment->humdOut;
        Test::avgHumdIn += Test::currentTrial->environment->humdIn;

        //Serial.println(Test::avgPart03um);//debugging
        delete Test::currentTrial;//dont overload memory
    }
    Test::avgTrials();//create average
    return Test::csvTest();//retrun string csv
}

void Test::avgTrials(){ //update each variable to its final state, divided by the 5 trials
    Test::avgPart03um /= 5.0;
    Test::avgPart05um /= 5.0;
    Test::avgPart10um /= 5.0;
    Test::avgPart25um /= 5.0;
    Test::avgPart50um /= 5.0; 
    Test::avgPart100um /= 5.0;
    Test::avgTempOut /= 5.0;
    Test::avgTempIn /= 5.0;
    Test::avgPresOut /= 5.0;
    Test::avgPresIn /= 5.0;
    Test::avgHumdOut /= 5.0;
    Test::avgHumdIn /= 5.0;
}


float Test::deltaPres(){//how much greater pressure is inside vs outside
    return Test::avgPresIn - Test::avgPresOut;
}

String Test::csvTest(){//string output to be tacked onto others - used simply to compartmentalize
    String csv = String(Test::avgPart03um);
    csv.concat(","); csv.concat(Test::avgPart05um);
    csv.concat(","); csv.concat(Test::avgPart10um);
    csv.concat(","); csv.concat(Test::avgPart25um);
    csv.concat(","); csv.concat(Test::avgPart50um);
    csv.concat(","); csv.concat(Test::avgPart100um);
    csv.concat(","); csv.concat(String(Test::avgTempOut, 6));//in strings to specify precision
    csv.concat(","); csv.concat(String(Test::avgTempIn, 6));
    csv.concat(","); csv.concat(String(Test::avgPresOut, 6));
    csv.concat(","); csv.concat(String(Test::avgPresIn, 6));
    csv.concat(","); csv.concat(String(Test::avgHumdOut, 6));
    csv.concat(","); csv.concat(String(Test::avgHumdIn, 6));
    csv.concat(","); csv.concat(String(Test::deltaPres(), 6));
    csv.concat(","); csv.concat(Test::getInfectAeroSum());
    return csv;
}

uint32_t Test::getInfectAeroSum(){
    return Test::avgPart03um + Test::avgPart05um + Test::avgPart10um + Test::avgPart25um + Test::avgPart50um;
}

float Test::getPresOut(){
    return Test::avgPresOut;
}

float Test::getHumdOut(){
    return Test::avgHumdOut;
}

float Test::getTempOut(){
    return Test::avgTempOut;
}
