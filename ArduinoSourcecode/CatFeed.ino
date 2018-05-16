#include <Servo.h>

Servo servo;

/*----- Const -----*/
const int SERVO_MIN_POSITION = 1; //Posiçao minima do servo motor
const int SERVO_MAX_POSITION = 90; //Posiçao máxima do servo motor
const int FEED_TIME = 700; //Tempo, em milis, que a porta ficará aberta
const int SERVO_PIN = 11; //Pino aonde está o servo
const bool DEBUG_MODE = true; //Modo debug (habilita saida do monitor serial)

/*---- Global -----*/
int delayTime = FEED_TIME/SERVO_MAX_POSITION; 
int servoPosition = 0; //Posicao atual do servo
unsigned long currentTime;
unsigned long lastFeedTime = 0;
int feedOffse = 8000;
bool isCalibrated = false;
bool timeToFeed = false;

void setup() {
  servo.attach(SERVO_PIN);
  
  if(DEBUG_MODE){
    Serial.begin(9600);
  }
}

double TIMERS[4][3] = {
  {6, 0, 0},
  {12, 0, 0},
  {18, 0, 0},
  {22, 0, 0}
};

unsigned long CLOCK_LastTick = 0;
double CLOCK_CurrentTime[3] = { 14, 14, 30};

void defineClock(){
  double dMillis = (double) millis()/1000.0;
  double dLastTick = (double)CLOCK_LastTick/1000.0;
 
  CLOCK_CurrentTime[2] = CLOCK_CurrentTime[2] + (dMillis - dLastTick);
  if(CLOCK_CurrentTime[2] > 59){
    CLOCK_CurrentTime[2] = CLOCK_CurrentTime[2] - 60;
    CLOCK_CurrentTime[1] = CLOCK_CurrentTime[1]+1;

    if(CLOCK_CurrentTime[1] > 59){
      CLOCK_CurrentTime[1] = CLOCK_CurrentTime[1] - 60;
      CLOCK_CurrentTime[0] = CLOCK_CurrentTime[0]+1;

      if(CLOCK_CurrentTime[0] > 23){
        CLOCK_CurrentTime[0] = 0;
      }
    }
  }
  
  CLOCK_LastTick =millis();
}



/**
 * Insere uma mensagem debug no monitor serial
 */
void debug(String txt){
  if(DEBUG_MODE){
     Serial.println(txt);
  }
}

/**
 * Movimenta o servo, alimentando os gatos
 */
void feed(){
  for (servoPosition = SERVO_MIN_POSITION; servoPosition <= SERVO_MAX_POSITION; servoPosition += 1) { 
     servo.write(servoPosition);              
     delay(delayTime);                      
  }
   for (servoPosition = servoPosition; servoPosition >= SERVO_MIN_POSITION; servoPosition -= 1) { 
    servo.write(servoPosition);              
    delay(delayTime);                       
  }
  
}

/**
 * Reinicia o servo, para calibra-lo
 */
void calibrate(){
   servo.write(SERVO_MIN_POSITION);
   servo.write(SERVO_MIN_POSITION+10);
   servo.write(SERVO_MIN_POSITION);
   isCalibrated=true;
}


void loop() {
  defineClock();
  
  if(!isCalibrated){
    calibrate();
  }

  if(timeToFeed){
    feed();
    timeToFeed = false;
  }
  /*
   currentTime = millis();
   if(currentTime - lastFeedTime >= feedOffse){
     feed();
     lastFeedTime = currentTime;
   }
   //debug(String(currentTime - lastFeedTime));*/
   
   debug(String((int)CLOCK_CurrentTime[0]) + ":" + String((int)CLOCK_CurrentTime[1]) + ":" + String((int)CLOCK_CurrentTime[2]));
   delay(1000);
}