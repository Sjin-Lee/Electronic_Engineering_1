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

int PWMRedValue=0;
int PWMGreenValue=0;
int PWMBlueValue=0;
int PWMOnBoardValue=0;

int LEDState=0;

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

}

void loop()
{
    ledcWrite(PWM_CH_LED_RED,PWMRedValue);
    ledcWrite(PWM_CH_LED_GREEN,PWMGreenValue);
    ledcWrite(PWM_CH_LED_ONBOARD,PWMOnBoardValue);
    
    if(PWMRedValue==255)LEDState=1;
    if(PWMRedValue==0)LEDState=0;
    
    if(LEDState>0)
    {
        PWMRedValue--;
        PWMGreenValue--;
        PWMOnBoardValue--;
    }
    else
    {
    
        PWMRedValue++;
        PWMGreenValue++;
        PWMOnBoardValue++;
    }
  
    delay(10);

}
