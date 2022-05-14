

/* Pin & Function Definitions*/
/* Pin & Function Definitions*/
#define HEAD_SENSOR 15
#define LEFT_ECHO 35
#define LEFT_TRIG 25
#define RIGHT_ECHO 17
#define RIGHT_TRIG 16
#define FRONT_ECHO 34 
#define FRONT_TRIG 2
#define SCREEN_SCK 22
#define SCREEN_SDA 21
#define AUDIO_PIN 0
#define EN1 5
#define IN1 13
#define IN2 32

#define IN3 14
#define IN4 26
#define EN2 4

/*Edit at 2022/4/24 20:30 */

#define HAPPY 1
#define SHY 0

#define LEFT 3
#define RIGHT 2
#define FRONT 1
#define STOP 0
#define BACK -1

#define MOVEONCE 1000
#define TURN_DELAYms 600

using namespace std;

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

    pinMode(EN1,OUTPUT);
    pinMode(EN2,OUTPUT);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);
}

void loop(){
    MOVE(FRONT);
    delay(2000);
    MOVE(BACK);
}

inline void MOVE(int direction){
    if(direction==STOP){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,HIGH);
        return;
    }
    //digitalWrite(EN1,HIGH);
    //digitalWrite(EN2,HIGH);
    ledcAttachPin()
    if(direction==FRONT){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
        delay(MOVEONCE);
    }
    if(direction==BACK){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
        delay(MOVEONCE);
    }
    if(direction==LEFT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
        delay(TURN_DELAYms);
        MOVE(STOP);
    }
    if(direction==RIGHT){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
        delay(TURN_DELAYms);
        MOVE(STOP);
    }
    return;
}

//20220424 12:30 测试成功 两个电机可以转动
//左轮焊接稍不稳
