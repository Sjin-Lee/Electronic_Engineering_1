#define LMOTO 33
#define RMOTO 39 //SVN

unsigned int Lmoto=0;
unsigned int Rmoto=0;

void setup(){
    Serial.begin(115200);
    pinMode(LMOTO,INPUT);
    pinMode(RMOTO,INPUT);
    
    attachInterrupt(LMOTO,left_motor,RISING);
    attachInterrupt(RMOTO,right_motor,RISING);

}

void loop(){
    Serial.print("Left:");
    Serial.print(Lmoto);
    Serial.print("   Rmoto:");
    Serial.println(Rmoto);
    delay(1000);
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
