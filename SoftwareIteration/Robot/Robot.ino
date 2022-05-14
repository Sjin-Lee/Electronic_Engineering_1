#include"Definations.h"
using namespace std;
SSD1306 display(0x3c, SCREEN_SDA, SCREEN_SCK);
WiFiServer server(80);
int LeftDistance,RightDistance,FrontDistance,minDistance,maxDistance;
int BrightVolt;
char buffer[20];
int TowardsNow=STOP;
inline int Getdistance(int,int);
inline void RemoteControl();
inline void ShowMouth(int);
inline void PlaySound(int);
inline void MOVE(int);
inline void PWMMOVE(int,int,int);
inline void SetupWifi();

unsigned int Lmoto=0;
unsigned int Rmoto=0;
unsigned int Lspeed=0;
unsigned int Rspeed=0;

int Stadirection;
int StaSpeed;

int DIRECTION=STOP;
int LSPEED = 0;
int RSPEED = 0;

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

    pinMode(LMOTO,INPUT);
    pinMode(RMOTO,INPUT);
    
    attachInterrupt(LMOTO,left_motor,RISING);
    attachInterrupt(RMOTO,right_motor,RISING);
    
    ledcSetup(PWM_EN1_CH,PWM_FREQ_HZ,PWM_RESOLUTION);
    ledcSetup(PWM_EN2_CH,PWM_FREQ_HZ,PWM_RESOLUTION);

    ledcAttachPin(EN1,PWM_EN1_CH);
    ledcAttachPin(EN2,PWM_EN2_CH);

    xTaskCreate(
        DetectDistance,
        "detectron",
        4096,
        NULL,
        1,
        NULL);
    
    xTaskCreate(
        WhileMove,
        "moving",
        4096,
        NULL,
        1,
        NULL);
}

void loop(){
        BrightVolt=analogRead(HEAD_SENSOR)*3.3/4096;
        if(BrightVolt==1||BrightVolt==2){
            //PlaySound(SHY);            
        }
        else if(BrightVolt==3){
            bool Flag=true;
            for(int i=1;i<=300;i++){
                BrightVolt=analogRead(HEAD_SENSOR)*3.3/4096;
                if(BrightVolt!=3){
                    Flag=false;
                    break;
                }
                delay(10);
            }
            if(Flag){
                SetupWifi();
                RemoteControl();
            }
        }
}

void DetectDistance(void * parameter){
        FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
        LeftDistance=Getdistance(LEFT_TRIG,LEFT_ECHO);
        RightDistance=Getdistance(RIGHT_TRIG,RIGHT_ECHO);
        minDistance=min(LeftDistance, min (RightDistance,FrontDistance) );
        maxDistance=max(LeftDistance, max (RightDistance,FrontDistance));
        /* Self Protection */
        if(maxDistance<=SAFETY_DISTANCE){
            DIRECTION = STOP;
        }
        /* End of Self Protection*/

        else if(minDistance<=50){
            ShowMouth(SHY);

            if(minDistance==FrontDistance)Stadirection=BACK;
            else if(minDistance==LeftDistance)Stadirection=LEFT;
            else if(minDistance==RightDistance)Stadirection=RIGHT;
            
            if(minDistance>=40)StaSpeed=SLOW;
            else if(minDistance>=30)StaSpeed=NORMAL;
            else if(minDistance>=20)StaSpeed=FAST;
            else if(minDistance>=10)StaSpeed=FASTEST;
        
            MOVE(Stadirection);

        }
        else{
            ShowMouth(HAPPY);
            MOVE(STOP);
            delay(500);
        }
        delay(400);
}

void WhileMove(void * parameter){
    TowardsNow=DIRECTION;
    if(DIRECTION==STOP){
        digitalWrite(EN1,LOW);
        digitalWrite(EN2,LOW);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW);
        return;
    }
    if (DIRECTION==FORCE)
    {
        digitalWrite(EN1,HIGH);
        digitalWrite(EN2,HIGH);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW);
        return;
    }
    if(DIRECTION==BACK){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);

    }
    if(DIRECTION==FRONT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
    }
    if(DIRECTION==LEFT){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
    }
    if(DIRECTION==RIGHT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
    }

    ledcWrite(PWM_EN1_CH,LSPEED);
    ledcWrite(PWM_EN2_CH,RSPEED);

}

void left_motor()            //触发函数
{
  Lmoto++;	
  if(Lmoto>=99999999)
    Lmoto=0;
}

void right_motor()            //触发函数
{
  Rmoto++;	
  if(Rmoto>=99999999)
    Rmoto=0;
}

inline void MOVE(int direction){
    TowardsNow=direction;
    if(direction==STOP){
        digitalWrite(EN1,LOW);
        digitalWrite(EN2,LOW);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW);
        return;
    }
    if (direction==FORCE)
    {
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
        return;
    }
    if(direction==FRONT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
        return;
    }
    if(direction==LEFT){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
        return;
    }
    if(direction==RIGHT){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
        return;
    }
    return;
}

inline void SetupWifi(){
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    server.begin();
    display.clear();
    display.setFont(ENGLISHFONT);
    display.drawString(0,0,"WiFI AP has been set up");
    display.drawString(0,16,"SSID : ShyRobot");
    display.drawString(0,32,"PassWord : group666" );
    display.drawString(0,48,"Waiting for Connection...");
    display.display();
    return;
}

inline void RemoteControl(){
    bool Flag=true;
    while(Flag){
        WiFiClient client = server.available();   // listen for incoming clients
        if (client){                 
            display.clear();            // if you get a client,
            display.setFont(ArialMT_Plain_24);
            display.drawString(0,0,"Connected");
            display.display();
            String currentLine = "";              // make a String to hold incoming data from the client
            while (client.connected()) {            // loop while the client's connected
                if (client.available()) {             // if there's bytes to read from the client,
                    char c = client.read();
                    if (c == '\n') {                    // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0) {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();
                            client.print("Click <a href=\"/F\">here</a> to MOVE FORWARD.<br>");
                            client.print("Click <a href=\"/L\">here</a> to TURN LEFT.<br>");
                            client.print("Click <a href=\"/R\">here</a> to TURN RIGHT.<br>");
                            client.print("Click <a href=\"/B\">here</a> to MOVE BACKWARD.<br>");
                            client.print("<br><br>Click <a href=\"/S\">here</a> to STOP Remote Control.<br>");
                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        } 
                        else{ currentLine = "";}    // if you got a newline, then clear currentLine:
                    }
                    else if (c != '\r') { currentLine += c; }

                    if (currentLine.endsWith("GET /L")) {
                        MOVE(LEFT);
                        display.clear();            // if you get a client,
                        display.setFont(ArialMT_Plain_24);
                        display.drawString(0,0,"Connected");
                        display.drawString(0,48,"LEFT");
                        display.display();
                        delay(MOVEONCE);
                        MOVE(STOP);
                        
                    }
                    if (currentLine.endsWith("GET /R")) {
                        MOVE(RIGHT);
                        display.clear();            // if you get a client,
                        display.setFont(ArialMT_Plain_24);
                        display.drawString(0,0,"Connected");
                        display.drawString(0,48,"RIGHT");
                        display.display();
                        delay(MOVEONCE);
                        MOVE(STOP);
                    }
                    if(currentLine.endsWith("GET /F")){
                        MOVE(FRONT);
                        display.clear();            // if you get a client,
                        display.setFont(ArialMT_Plain_24);
                        display.drawString(0,0,"Connected");
                        display.drawString(0,48,"FRONT");
                        display.display();
                        delay(MOVEONCE);
                        MOVE(STOP);
                    }
                    if(currentLine.endsWith("GET /B")){
                        MOVE(BACK);
                        display.clear();            // if you get a client,
                        display.setFont(ArialMT_Plain_24);
                        display.drawString(0,0,"Connected");
                        display.drawString(0,48,"BACK");
                        display.display();
                        delay(MOVEONCE);
                        MOVE(STOP);
                    }
                    if(currentLine.endsWith("GET /S")){
                        display.clear();            // if you get a client,
                        display.setFont(ArialMT_Plain_24);
                        display.drawString(0,0,"Wifi");
                        display.drawString(0,48,"Shutdown");
                        display.display();
                        MOVE(STOP);
                        Flag=false;
                    }
            
                }
            }
            // close the connection:
            client.stop();
            display.clear();
            display.setFont(ArialMT_Plain_16);
            display.drawString(0,0,"No Command");
            display.display();
            delay(1000);
        }
    }
    return; 
}

inline void ShowMouth(int mood){
    display.clear();
    display.setFont(ENGLISHFONT);
    display.drawString(0,12,itoa(BrightVolt,buffer,10));
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
        display.drawXbm( 0,15, emotion_width,emotion_height , HappyMouth);
    }
    if(mood==SHY){
        display.drawString(0,0,"Shy");
        display.drawXbm( 0,15, emotion_width, emotion_height , ShyMouth);
    }
    if(mood==UNSAFE){
        display.drawString(0,0,"Unsafe");
        display.setFont(ArialMT_Plain_24);
        display.drawString(0,30,"Too Near!!");
    }
    display.display();

}