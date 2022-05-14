#include<Arduino.h>
#include<U8g2lib.h>
#include<Wire.h>
#include<SSD1306.h>
#include<bits/stdc++.h>
#include<cstdlib>
#include<WiFi.h>
#include"Emotion.h"
#include"Shymusic.h"
#include"UltraSonic.h"


#define PWM_FREQ_HZ 6000
#define PWM_RESOLUTION 8
#define PWM_EN1_CH 0
#define PWM_EN2_CH 1

#define HEAD_SENSOR 36  //SVP
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

#define HAPPY 1
#define SHY 0
#define UNSAFE 9

#define LEFT 3
#define RIGHT 2
#define FRONT 1
#define STOP 0
#define BACK -1
#define FORCE 9

#define ENGLISHFONT ArialMT_Plain_10

#define SAFETY_DISTANCE 8

#define MOVEONCE 150
#define TURN_DELAYms 150

const char *ssid = "ShyRobot";
const char *password = "group666";

#define LMOTO 33
#define RMOTO 39 //SVN

#define SLOW 170
#define NORMAL 200
#define FAST 220
#define FASTEST 250

#define emotion_width 128
#define emotion_height 47