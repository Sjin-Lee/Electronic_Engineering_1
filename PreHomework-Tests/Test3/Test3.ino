#include <Arduino.h>
//Key：Yellow=Red+Green
#define PWM_FREQ_HZ        2000
#define PWM_CH_LED_RED     0
#define PWM_CH_LED_GREEN   1
#define PWM_CH_LED_BLUE    2
#define PWM_CH_LED_ONBOARD 3
#define PWM_RESOLUTION     8

#define PIN_LED_RED     16
#define PIN_LED_GREEN   17
#define PIN_LED_BLUE    18
#define PIN_LED_ONBOARD 2

#define PIN_LightRecv 34

int PWMRedValue=0;
int PWMGreenValue=0;
int PWMBlueValue=0;
int PWMOnBoardValue=0;

void setup()
{
    Serial.begin(115200);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_LED_ONBOARD, OUTPUT);

    ledcSetup(PWM_CH_LED_RED,     PWM_FREQ_HZ, PWM_RESOLUTION); 
    ledcSetup(PWM_CH_LED_GREEN,   PWM_FREQ_HZ, PWM_RESOLUTION); 
    ledcSetup(PWM_CH_LED_BLUE,    PWM_FREQ_HZ, PWM_RESOLUTION); 
    ledcSetup(PWM_CH_LED_ONBOARD, PWM_FREQ_HZ, PWM_RESOLUTION); 
  
    ledcAttachPin(PIN_LED_RED,    PWM_CH_LED_RED);   
    ledcAttachPin(PIN_LED_GREEN,  PWM_CH_LED_GREEN);
    ledcAttachPin(PIN_LED_BLUE,   PWM_CH_LED_BLUE);
    ledcAttachPin(PIN_LED_ONBOARD,PWM_CH_LED_ONBOARD);

    pinMode(PIN_LightRecv,INPUT);
}

void loop()
{
    ledcWrite(PWM_CH_LED_RED,PWMRedValue);
    ledcWrite(PWM_CH_LED_GREEN,PWMGreenValue);
    ledcWrite(PWM_CH_LED_ONBOARD,PWMOnBoardValue);
    int Bright=0;
    Bright=analogRead(PIN_LightRecv);
    Serial.print("P34引脚的电压 = ");
    Serial.print(Bright);
    Serial.print("    ");
    Serial.print(Bright*3.3/4096);
    Serial.print(" V   ");
    if(Bright<100)Bright=100;
    if(Bright>1500)Bright=1500;
    //Bright 100-1500映射到0-255之间的值
    Bright=(Bright-100)*255/(1500-100);
    Serial.print("Bright=   ");
    Serial.println(Bright);
    PWMBlueValue=Bright;
    PWMGreenValue=Bright;
    PWMRedValue=Bright;
    PWMOnBoardValue=Bright;

    delay(100);
}
