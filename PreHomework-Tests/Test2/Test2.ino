#include "numberTTS.h"
#define AUDIO_PIN 0
#define DAC_DATA_LEN 92868
int  DACDataPointer=0;
int RepeatFlag=0;

int Looptime=0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Begin!");
    pinMode(AUDIO_PIN, OUTPUT);
}

void loop()
{
    delayMicroseconds(30);
    dacWrite(AUDIO_PIN, rawData[DACDataPointer]-50 );
    Serial.println(rawData[DACDataPointer]-50);
    DACDataPointer++;
    if(DACDataPointer>=DAC_DATA_LEN)
    {
        DACDataPointer = 0;
        RepeatFlag=1;
    }
    if(RepeatFlag ==1)
    {
        RepeatFlag=0;
        Looptime = millis() - Looptime;
  
        Looptime = millis();
    }

}
