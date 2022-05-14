#include<Arduino.h>
#include<U8g2lib.h>
#include<Wire.h>
#include<SSD1306.h>

/* Pin & Function Definitions*/
#define HEAD_SENSOR 15
#define LEFT_ECHO 14
#define LEFT_TRIG 12
#define RIGHT_ECHO 17
#define RIGHT_TRIG 16
#define FRONT_ECHO 4
#define FRONT_TRIG 0
//#define BACK_ECHO undefined
//#define BACK_TRIG undefined
#define SCREEN_SCK 22
#define SCREEN_SDA 21
#define AUDIO_PIN 25
#define IN1 35
#define IN2 32
#define IN3 33
#define IN4 26
#define EN1 34
#define EN2 27

#define HAPPY 1
#define SHY 0

#define LEFT 3
#define RIGHT 2
#define FRONT 1
#define STOP 0
#define BACK -1

#define ENGLISHFONT ArialMT_Plain_24

#define MOVEONCE 1000
#define TURN_DELAYms 600

const char *ssid = "ShyRobot";
const char *password = "group666";

using namespace std;

SSD1306 display(0x3c, SCREEN_SDA, SCREEN_SCK);

int LeftDistance,RightDistance,FrontDistance,minDistance;
int LightR;
inline int Getdistance(int,int);
inline void BeHappy();
inline void BeShy();
inline void RemoteControl();
inline void ShowMouth(int);
inline void PlaySound(int);
inline void MOVE(int);
inline void TURN(int);
inline void SetupWifi();

/* End of Definitions*/


void setup(){

    pinMode(HEAD_SENSOR,INPUT);
    pinMode(LEFT_ECHO,INPUT);
    pinMode(LEFT_TRIG,OUTPUT);
    pinMode(RIGHT_ECHO,INPUT);
    pinMode(RIGHT_TRIG,OUTPUT);
    pinMode(FRONT_ECHO,INPUT);
    pinMode(FRONT_TRIG,OUTPUT);
    Serial.begin(115200);
    display.init();
    
}

void loop(){
        FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
        LeftDistance=Getdistance(LEFT_TRIG,LEFT_ECHO);
        RightDistance=Getdistance(RIGHT_TRIG,RIGHT_ECHO);
        Serial.println(FrontDistance);
        Serial.println(LeftDistance);
        Serial.println(RightDistance);
        delay(3000);
        minDistance=min(LeftDistance, min (RightDistance,FrontDistance) );
        if(minDistance<=15){
            ShowMouth(SHY);
        }
        else{
            ShowMouth(HAPPY);
        }
}


inline int Getdistance(int trig, int echo){
    long IntervalTime=0;
    digitalWrite(trig, 1);
    delayMicroseconds(25);
    digitalWrite(trig, 0);
    IntervalTime=pulseIn(echo, HIGH);
    float S=IntervalTime/58.00;
    return S;
}

inline void ShowMouth(int mood){ //to be continued
    if(mood==HAPPY){
        display.clear();
        display.setFont(ENGLISHFONT);
        display.drawString(0,0,"Happy");
        display.display();
    }

    if(mood==SHY){
        display.clear();
        display.setFont(ENGLISHFONT);
        display.drawString(0,0,"Shy");
        display.display();
        
    }
}

//20220416 16:00 测试成功  三个超声波传感器能够测距 距离不同 显示的文字不同