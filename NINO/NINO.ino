#include <PS4Controller.h>

int INITIALSPEED = 64;
int MAXSPEEDA = 800;
int MAXSPEEDB = 800;
int speedA = 0;
int speedB = 0;
int throttle = 0;
bool majuA = true;
bool majuB = true;

const int freq = 5000;
const int resolution = 10; //Resolution 8, 10, 12, 15

// pins for motor 1
#define RPWM_CH1 0
#define RPWM_1 16 // define pin 3 for RPWM pin (output)
#define R_EN_1 17 // define pin 2 for R_EN pin (input)
#define R_IS_1 5 // define pin 5 for R_IS pin (output)

#define LPWM_CH1 1
#define LPWM_1 6 // define pin 6 for LPWM pin (output)
#define L_EN_1 7 // define pin 7 for L_EN pin (input)
#define L_IS_1 8 // define pin 8 for L_IS pin (output)
// motor 1 pins end here

// pins for motor 2
#define RPWM_CH2 2
#define RPWM_2 9 // define pin 9 for RPWM pin (output)
#define R_EN_2 10 // define pin 10 for R_EN pin (input)
#define R_IS_2 12 // define pin 12 for R_IS pin (output)

#define LPWM_CH2 3
#define LPWM_2 11 // define pin 11 for LPWM pin (output)
#define L_EN_2 13 // define pin 7 for L_EN pin (input)
#define L_IS_2 14 // define pin 8 for L_IS pin (output)
// motor 2 pins end here

#define CW 1 //
#define CCW 0 //
#define debug 1 //

#include <RobojaxBTS7960.h>
RobojaxBTS7960 motorA(R_EN_1,RPWM_CH1,R_IS_1, L_EN_1,LPWM_CH1,L_IS_1,debug);//define motor 1 object
RobojaxBTS7960 motorB(R_EN_2,RPWM_CH2,R_IS_2, L_EN_2,LPWM_CH2,L_IS_2,debug);//define motor 2 object and the same way for other motors



void setup() { 
  Serial.begin(115200);
  PS4.begin("40:99:22:d3:2e:ac");
  Serial.println("Ready.");

  ledcSetup(RPWM_CH1, freq, resolution);
  ledcAttachPin(RPWM_1, RPWM_CH1);

  ledcSetup(LPWM_CH1, freq, resolution);
  ledcAttachPin(LPWM_1, LPWM_CH1);

  ledcSetup(RPWM_CH2, freq, resolution);
  ledcAttachPin(RPWM_2, RPWM_CH2);
  
  ledcSetup(LPWM_CH2, freq, resolution);
  ledcAttachPin(LPWM_2, LPWM_CH2);
  
  motorA.begin();
  motorB.begin();
}

void loop() {
  if (PS4.isConnected()) {
    throttle = PS4.R2Value()-PS4.L2Value(); 
    
    if (throttle<49 and throttle>-50) { //buat ngarahin robot + yaw
      speedA = map(PS4.LStickX(), 0, 127, 0, MAXSPEEDA*1);
      speedB = map(-PS4.LStickX(), 0, 127, 0, MAXSPEEDB*1);
    }
    
    else { //buat mode setir biasa
      speedA = map(INITIALSPEED+PS4.LStickY()+PS4.LStickX()/2, 0, 268, 0, throttle);
      speedB = map(INITIALSPEED+PS4.LStickY()-PS4.LStickX()/2, 0, 268, 0, throttle);
  
      speedA = map(speedA, 0, 255, 0, MAXSPEEDA);
      speedB = map(speedB, 0, 255, 0, MAXSPEEDB);
  
      }
    }
  else{
  Serial.println("disconnected");
  delay(100);
  }
  
  if (speedA>20){
    motorA.rotate(speedA, CCW);
  }
  else if (speedA<-20){
    motorA.rotate(-speedA, CW);
  }
  else{
    motorA.stop();
  }
  
  if (speedB>20){
    motorB.rotate(speedB, CW);
  }
  else if (speedB<-20){
    motorB.rotate(-speedB, CCW);
  }
  else{
    motorB.stop();
  }
  
  Serial.println(speedA);
  Serial.println(speedB);
  Serial.println("");
  delay(100);
}





  
