#include "Zero.h"
#include "One.h"
#include "Two.h"
#include "Four.h"
#include "Seven.h"
#include "Nine.h"

#define AUDIO_PIN 25

#define ZeroLen 30742
#define OneLen 25642
#define TwoLen 23942
#define ThreeLen 20462
#define FourLen 27392
#define FiveLen 27052
#define SixLen 24382
#define SevenLen 27342
#define EightLen 27342
#define NineLen 23942

#define SwiftValue 80

void setup()
{
    pinMode(AUDIO_PIN, OUTPUT);
}

void PlayStudentNumber(long long number){
    while (number >10){
        PlayStudentNumber(number/10);
        number = number%10;
    }
    switch (number){
        case 0:
            for(int Pointer=0;Pointer<ZeroLen;Pointer++){
                dacWrite(AUDIO_PIN, ZeroData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
        case 1:
            for(int Pointer=0;Pointer<OneLen;Pointer++){
                dacWrite(AUDIO_PIN, OneData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
        case 2:
            for(int Pointer=0;Pointer<TwoLen;Pointer++){
                dacWrite(AUDIO_PIN, TwoData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
        case 3:
            //for(int Pointer=0;Pointer<ThreeLen;Pointer++){
            //    dacWrite(AUDIO_PIN, ThreeData[Pointer]-SwiftValue);
            //    delayMicroseconds(30);
            //}
            break;
        case 4:
            for(int Pointer=0;Pointer<FourLen;Pointer++){
                dacWrite(AUDIO_PIN, FourData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
        case 5:
            //for(int Pointer=0;Pointer<FiveLen;Pointer++){
            //    dacWrite(AUDIO_PIN, FiveData[Pointer]-SwiftValue);
            //    delayMicroseconds(30);
            //}
            break;
        case 6:
            //for(int Pointer=0;Pointer<SixLen;Pointer++){
            //    dacWrite(AUDIO_PIN, SixData[Pointer]-SwiftValue);
            //    delayMicroseconds(30);
            //}
            break;
        case 7:
            for(int Pointer=0;Pointer<SevenLen;Pointer++){
                dacWrite(AUDIO_PIN, SevenData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
        case 8:
            //for(int Pointer=0;Pointer<EightLen;Pointer++){
            //    dacWrite(AUDIO_PIN, EightData[Pointer]-SwiftValue);
            //    delayMicroseconds(30);
            //}
            break;
        case 9:
            for(int Pointer=0;Pointer<NineLen;Pointer++){
                dacWrite(AUDIO_PIN, NineData[Pointer]-SwiftValue);
                delayMicroseconds(30);
            }
            break;
    }
    
    delayMicroseconds(100);
}
void loop()
{
    delayMicroseconds(3000);
    long long StudentNumber=2021270901014;
    PlayStudentNumber(StudentNumber);
}
