#include"Defination.h"
using namespace std;
SSD1306 display(0x3c, SCREEN_SDA, SCREEN_SCK);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int LeftDistance;
int RightDistance;
int FrontDistance;
int minDistance;
int maxDistance;
float BrightVolt;
char buffer[20];

unsigned int Lmoto=0;
unsigned int Rmoto=0;

int Stadirection,StaSpeed;

int DIRECTION=STOP;
int LSPEED=150;
int RSPEED=150;
bool Flag=true;
int MOOD=HAPPY;

inline void RemoteControl();
void left_motor(){Lmoto++;}
void right_motor(){Rmoto++;}

void MOVE(){
    if(DIRECTION==STOP)
    {
        digitalWrite(EN1,LOW);
        digitalWrite(EN2,LOW);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW); 
    }
    if(DIRECTION==FORCE)
    {
        digitalWrite(EN1,HIGH);
        digitalWrite(EN2,HIGH);
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,LOW);
    }
    
    if(DIRECTION==BACK)
    {
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
    }
    if(DIRECTION==FRONT)
    {
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
    }
    if(DIRECTION==LEFT)
    {
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
    }
    if(DIRECTION==RIGHT)
    {
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
    }
    
    ledcWrite(PWM_EN1_CH,LSPEED);
    ledcWrite(PWM_EN2_CH,RSPEED*1.05);
}

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

    // attachInterrupt(LMOTO,left_motor,FALLING);
    // attachInterrupt(RMOTO,right_motor,FALLING);
     
    ledcSetup(PWM_EN1_CH,PWM_FREQ_HZ,PWM_RESOLUTION);
    ledcSetup(PWM_EN2_CH,PWM_FREQ_HZ,PWM_RESOLUTION);

    ledcAttachPin(EN1,PWM_EN1_CH);
    ledcAttachPin(EN2,PWM_EN2_CH);

    pinMode(15,OUTPUT);
    digitalWrite(15,HIGH);

}

void loop(){

    display.clear();
    display.setFont(ENGLISHFONT);
    display.drawString(0,12,itoa( int(floor( BrightVolt*10 )) ,buffer,10));
    display.drawString(40,0,itoa(LeftDistance,buffer,10));
    display.drawString(60,0,itoa(FrontDistance,buffer,10));
    display.drawString(80,0,itoa(RightDistance,buffer,10));
    if(DIRECTION==STOP){display.drawString(100,0,"STOP");}
    if(MOOD==HAPPY)
    {
        display.drawString(0,0,"Happy");
        display.drawXbm( 0,15, emotion_width,emotion_height , HappyMouth);
    }
    if(MOOD==SHY)
    {
        display.drawString(0,0,"Shy");
        display.drawXbm( 0,15, emotion_width, emotion_height , ShyMouth);
    }
    if(MOOD==UNSAFE)
    {
        display.drawString(0,0,"Unsafe");
        display.setFont(ArialMT_Plain_24);
        display.drawString(0,30,"Too Near!!");
    }
    display.display();

    BrightVolt=analogRead(HEAD_SENSOR)*3.3/4096;
    if(BrightVolt>1.4)
    {
        if(BrightVolt<=2.5)
        {
            MOOD=SHY;
            for(int i=0;i<AudioLength;i++)
            {
                dacWrite(AUDIO_PIN,rawData[i]-SwiftValue);
                delayMicroseconds(25);
            }
        }
        else
        {
            Flag=true;
            DIRECTION=STOP;
            MOVE();
            MOOD=HAPPY;
            for(int i=1;i<=300;i++)
            {
                BrightVolt=analogRead(HEAD_SENSOR)*3.3/4096;
                if(BrightVolt<=2.3)
                {
                    Flag=false;
                    break;
                }
                delay(10);
            }
            if(Flag)RemoteControl();
        }
    }
    
    FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
    LeftDistance=Getdistance(LEFT_TRIG,LEFT_ECHO);
    RightDistance=Getdistance(RIGHT_TRIG,RIGHT_ECHO);
    minDistance=min(LeftDistance, min (RightDistance,FrontDistance) );
    maxDistance=max(LeftDistance, max (RightDistance,FrontDistance));

    if(maxDistance<=SAFETY_DISTANCE)
    {
        DIRECTION = FORCE;
        MOOD=UNSAFE;
        for(int i=0;i<AudioLength;i++)
        {
            dacWrite(AUDIO_PIN,rawData[i]-SwiftValue);
            delayMicroseconds(25);
        }
        delay(1000);
        DIRECTION=FRONT;
        MOVE();
        delay(500);
        DIRECTION=STOP;
        MOVE();
    }
    else if(minDistance<=40)
    {
        MOOD=SHY;
        if(DIRECTION==STOP||DIRECTION==FORCE){
            for(int i=0;i<AudioLength;i++)
            {
                dacWrite(AUDIO_PIN,rawData[i]-SwiftValue);
                delayMicroseconds(25);
            }
        }
        if(minDistance>=35)StaSpeed=SLOW;
        else if(minDistance>=26)StaSpeed=NORMAL;
        else if(minDistance>=18)StaSpeed=FAST;
        else if(minDistance>=SAFETY_DISTANCE)StaSpeed=FASTEST;

        if(minDistance==FrontDistance)
        {
            LSPEED=StaSpeed;
            RSPEED=StaSpeed;
            DIRECTION=BACK;
            MOVE();
        }
        else if(minDistance==LeftDistance)
        {
            LSPEED=NORMAL;
            RSPEED=NORMAL;            
            DIRECTION=LEFT;
            MOVE();
            delay(TURN_DELAYms);
            // Lmoto=0;Rmoto=0;
            // while(Lmoto<=26||Rmoto<=26)delay(1);
            LSPEED=StaSpeed;
            RSPEED=StaSpeed;
            DIRECTION=FORCE;
            MOVE();
            DIRECTION=BACK;
            MOVE();
        }
        else if(minDistance==RightDistance)
        {
            LSPEED=NORMAL;
            RSPEED=NORMAL;
            DIRECTION=RIGHT;
            MOVE();
            delay(TURN_DELAYms);
            // Lmoto=0;Rmoto=0;
            // while(Lmoto<=26||Rmoto<=26)delay(1);
            LSPEED=StaSpeed;
            RSPEED=StaSpeed;
            DIRECTION=FORCE;
            MOVE();
            DIRECTION=BACK;
            MOVE();
        }
    }
    else if(minDistance>75)
    {
        MOOD=HAPPY;
        DIRECTION=STOP;
        MOVE();
        Flag=true;
        for(int i=1;i<=50;i++)
        {
            FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
            LeftDistance=Getdistance(LEFT_TRIG,LEFT_ECHO);
            RightDistance=Getdistance(RIGHT_TRIG,RIGHT_ECHO);
            minDistance=min(LeftDistance, min (RightDistance,FrontDistance) );
            if(minDistance<=50)
            {
                Flag=false;
                break;
            }
            delay(200);
        }
        if(Flag){
            display.clear();
            display.setFont(ENGLISHFONT);
            display.drawString(0,12,itoa( int(floor( BrightVolt*10 )) ,buffer,10));
            display.drawString(40,0,itoa(LeftDistance,buffer,10));
            display.drawString(60,0,itoa(FrontDistance,buffer,10));
            display.drawString(80,0,itoa(RightDistance,buffer,10));
            display.drawString(100,0,"DANC");
            display.drawString(0,0,"Happy");
            display.drawXbm( 0,15, emotion_width,emotion_height , HappyMouth);
            display.display();
            LSPEED=NORMAL;
            RSPEED=NORMAL;
            DIRECTION=LEFT;
            MOVE();
            delay(60);
            DIRECTION=RIGHT;
            MOVE();
            delay(120);
            DIRECTION=LEFT;
            MOVE();
            delay(60);
            DIRECTION=LEFT;
            MOVE();
            delay(60);
            DIRECTION=RIGHT;
            MOVE();
            delay(1000);
            DIRECTION=LEFT;
            MOVE();
            delay(1000);
            DIRECTION=FORCE;
            MOVE();
            delay(1000);
        }
    }
    else 
    {
        MOOD=HAPPY;
        DIRECTION=STOP;
        MOVE();
    }

    display.clear();
    display.setFont(ENGLISHFONT);
    display.drawString(0,12,itoa( int(floor( BrightVolt*10 )) ,buffer,10));
    display.drawString(40,0,itoa(LeftDistance,buffer,10));
    display.drawString(60,0,itoa(FrontDistance,buffer,10));
    display.drawString(80,0,itoa(RightDistance,buffer,10));
    if(DIRECTION==STOP){display.drawString(100,0,"STOP");}
    if(MOOD==HAPPY)
    {
        display.drawString(0,0,"Happy");
        display.drawXbm( 0,15, emotion_width,emotion_height , HappyMouth);
    }
    if(MOOD==SHY)
    {
        display.drawString(0,0,"Shy");
        display.drawXbm( 0,15, emotion_width, emotion_height , ShyMouth);
    }
    if(MOOD==UNSAFE)
    {
        display.drawString(0,0,"Unsafe");
        display.setFont(ArialMT_Plain_24);
        display.drawString(0,30,"Too Near!!");
    }
    display.display();

}

void processCarMovement(String inputValue){
    switch(inputValue.toInt())
    {
        case FRONT:
            FrontDistance=Getdistance(FRONT_TRIG,FRONT_ECHO);
            if(FrontDistance<=SAFETY_DISTANCE)
            {
                DIRECTION = FORCE;
                for(int i=0;i<AudioLength;i++)
                {
                    dacWrite(AUDIO_PIN,rawData[i]-SwiftValue);
                    delayMicroseconds(25);
                }
            }
            else 
            {
                DIRECTION=FRONT;
                MOVE();
            }
            break;
        
        case BACK:
            DIRECTION=BACK;
            MOVE();
            break;
        
        case LEFT:
            DIRECTION=LEFT;
            MOVE();
            break;
        
        case RIGHT:
            DIRECTION=RIGHT;
            MOVE();
            break;
        
        case STOP:
            DIRECTION=STOP;
            MOVE();
            break;

        case FORCE:
            DIRECTION=FORCE;
            MOVE();
            break;

        case 66:
            LSPEED=SLOW;
            RSPEED=SLOW;
            break;
        
        case 77:
            LSPEED=NORMAL;
            RSPEED=NORMAL;
            for(int i=0;i<CarEngineLen;i++){
                dacWrite(AUDIO_PIN,CarEngine[i]-SwiftValue);
                delayMicroseconds(25);
            }
            break;
        
        case 99:
            LSPEED=FASTEST;
            RSPEED=FASTEST;
            for(int i=0;i<CarEngineLen;i++){
                dacWrite(AUDIO_PIN,CarEngine[i]-SwiftValue);
                delayMicroseconds(25);
            }
            break;
    }
}

void handleRoot(AsyncWebServerRequest *request) 
{
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "File Not Found");
}

void onWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      //client->text(getRelayPinsStatusJson(ALL_RELAY_PINS_INDEX));
      break;
    case WS_EVT_DISCONNECT:
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
      processCarMovement("0");
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        processCarMovement(myData.c_str());
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}

inline void RemoteControl()
{
    LSPEED=FAST;
    RSPEED=FAST;
    for(int i=0;i<CarEngineLen;i++)
    {
        dacWrite(AUDIO_PIN,CarEngine[i]-SwiftValue);
        delayMicroseconds(25);
    }
    srand((unsigned)time(NULL));
    int Sw=rand()%10;
    int Gw=rand()%10;
    char EncryptedSSID[]={
        'S','h','y','R','o','b','o','t','a','b','\0'
    };
    EncryptedSSID[8]=char(Sw+'0');
    EncryptedSSID[9]=char(Gw+'0');
    WiFi.softAP(EncryptedSSID,password);
    IPAddress myIP = WiFi.softAPIP();

    display.clear();
    display.setFont(ENGLISHFONT);
    char PrintSSID[]={
        'W','i','-','F','i',' ',':',' ','S','h','y','R','o','b','o','t','a','b','\0'
    };
    PrintSSID[16]=char(Sw+'0');
    PrintSSID[17]=char(Gw+'0');

    display.drawString(0,0,PrintSSID);
    display.drawString(0,16,"PassWord : group666");
    display.drawString(0,32,"Web: 192.168.4.1 " );
    display.drawString(0,48,"Come and Play ! ");
    display.display();

    server.on("/", HTTP_GET, handleRoot);
    server.onNotFound(handleNotFound);
    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);
    server.begin();

    while(1)
    {
        ws.cleanupClients();
        delay(100);
    }
}