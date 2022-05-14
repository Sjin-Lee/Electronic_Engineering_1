#include<Arduino.h>
#include<U8g2lib.h>
#include<Wire.h>
#include<SSD1306.h>
#include<bits/stdc++.h>
#include<cstdlib>

/* Pin & Function Definitions*/

#define PWM_FREQ_HZ 2000
#define PWM_RESOLUTION 8
#define PWM_EN1_CH 0
#define PWM_EN2_CH 1

#define HEAD_SENSOR 36
#define LEFT_ECHO 35
#define LEFT_TRIG 25
#define RIGHT_ECHO 17
#define RIGHT_TRIG 16
#define FRONT_ECHO 34 
#define FRONT_TRIG 2
#define SCREEN_SCK 22
#define SCREEN_SDA 21
#define AUDIO_PIN 26
#define EN1 5
#define IN1 13
#define IN2 32

#define IN3 14
#define IN4 27
#define EN2 4

/*Edit at 2022/4/24 21:30 */

#define HAPPY 1
#define SHY 0
#define UNSAFE 9

#define LEFT 3
#define RIGHT 2
#define FRONT 1
#define STOP 0
#define BACK -1

#define ENGLISHFONT ArialMT_Plain_10

#define SAFETY_DISTANCE 8

#define MOVEONCE 1000
#define TURN_DELAYms 600

const char *ssid = "ShyRobot";
const char *password = "group666";

using namespace std;

SSD1306 display(0x3c, SCREEN_SDA, SCREEN_SCK);

int LeftDistance,RightDistance,FrontDistance,minDistance,maxDistance;
char buffer[20];
int TowardsNow=STOP;
inline int Getdistance(int,int);
inline void RemoteControl();
inline void ShowMouth(int);
inline void PlaySound(int);
inline void MOVE(int);
inline void SetupWifi();

/* End of Pins Variables and Definitions*/

void setup(){

    pinMode(HEAD_SENSOR,INPUT);
    pinMode(LEFT_ECHO,INPUT);
    pinMode(LEFT_TRIG,OUTPUT);
    pinMode(RIGHT_ECHO,INPUT);
    pinMode(RIGHT_TRIG,OUTPUT);
    pinMode(FRONT_ECHO,INPUT);
    pinMode(FRONT_TRIG,OUTPUT);

    pinMode(EN1,OUTPUT);
    pinMode(EN2,OUTPUT);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);

    digitalWrite(EN1,LOW);
    digitalWrite(EN2,LOW);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);

    display.init();
    display.clear();
    
}

void loop(){
        FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
        LeftDistance=Getdistance(LEFT_TRIG,LEFT_ECHO);
        RightDistance=Getdistance(RIGHT_TRIG,RIGHT_ECHO);
        minDistance=min(LeftDistance, min (RightDistance,FrontDistance) );
        maxDistance=max(LeftDistance, max (RightDistance,FrontDistance));
        /* Self Protection */
        if(maxDistance<=SAFETY_DISTANCE){
            MOVE(STOP);
            ShowMouth(UNSAFE);
        }
        /* End of Self Protection*/
        else if(minDistance<=40){
            ShowMouth(SHY);
            if(minDistance==FrontDistance){
                MOVE(BACK);
            }
            if(minDistance==LeftDistance){
                MOVE(LEFT);
                MOVE(BACK);
            }
            if(minDistance==RightDistance){
                MOVE(RIGHT);
                MOVE(BACK);
            }
        }
        else{
            ShowMouth(HAPPY);
            MOVE(STOP);
            delay(500);
        }
        delay(400);
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
    display.clear();
    display.setFont(ENGLISHFONT);
    display.drawString(40,0,itoa(LeftDistance,buffer,10));
    display.drawString(60,0,itoa(FrontDistance,buffer,10));
    display.drawString(80,0,itoa(RightDistance,buffer,10));
    if(TowardsNow==STOP){display.drawString(100,0,"STOP");}
    else if(TowardsNow==LEFT){display.drawString(100,0,"LEFT");}
    else if(TowardsNow==RIGHT){display.drawString(100,0,"RIGT");}
    else if(TowardsNow==FRONT){display.drawString(100,0,"FROT");}
    else if(TowardsNow==BACK){display.drawString(100,0,"BACK");}
    if(mood==HAPPY){
        display.drawString(0,0,"Happy");
    }
    if(mood==SHY){
        display.drawString(0,0,"Shy");
    }
    if(mood==UNSAFE){
        display.drawString(0,0,"Unsafe");
        display.setFont(ArialMT_Plain_24);
        display.drawString(0,30,"Too Near!!");
    }
    display.display();

}

inline void MOVE(int direction){
    TowardsNow=direction;
    if(direction==STOP){
        digitalWrite(EN1,HIGH);
        digitalWrite(EN2,HIGH);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW);
        return;
    }
    digitalWrite(EN1,HIGH);
    digitalWrite(EN2,HIGH);
    if(direction==BACK){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
    }
    if(direction==FRONT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
    }
    if(direction==LEFT){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
        delay(TURN_DELAYms);
        MOVE(STOP);
    }
    if(direction==RIGHT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
        delay(TURN_DELAYms);
        MOVE(STOP);
    }
    return;
}

/* 

   To do 20220424 23:30:

   Motor Run Stablily Smoothly and Correctly
   Multi-thread Program
   Screen Show Emotion
   Running Speed Control
   Audio Output
   
   Extention:
   Light-detection-sensor
   Remote Control

*/
