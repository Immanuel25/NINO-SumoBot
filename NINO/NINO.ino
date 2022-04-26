#include <PS4Controller.h>

int MAXSPEEDA = 800; // 800/1023 x100% duty cycle 
int MAXSPEEDB = 800; // 800/1023 x100% duty cycle
int INITIALSPEED = 64; 
int speedA = 0;
int speedB = 0;
int throttle = 0;
bool majuA = true;
bool majuB = true;

const int freq = 5000;
const int resolution = 10; //Resolution 8, 10, 12, 15

// pins for motor A
#define RPWM_CHA 0 // CHANNEL PWM FORWARD MOTOR A
#define RPWM_A 35 // GPIO18 (D18)
#define R_EN_A 38 // GPIO19 (D19)
#define R_IS_A 25 // GPIO16 (RX2)

#define LPWM_CHA 1 // CHANNEL PWM REVERSE MOTOR A
#define LPWM_A 42 // GPIO21 (D21)
#define L_EN_A 14 // GPIO25 (D25)
#define L_IS_A 27 // GPIO17 (TX2)
// motor A pins end here

// pins for motor B
#define RPWM_CHB 2 // CHANNEL PWM FORWARD MOTOR B
#define RPWM_B 15 // GPIO26 (D26)
#define R_EN_B 16 // GPIO27 (D27)
#define R_IS_B 39 // GPIO22 (D22)

#define LPWM_CHB 3 // CHANNEL PWM REVERSE MOTOR B
#define LPWM_B 12 // GPIO32 (D32)
#define L_EN_B 13 // GPIO33 (D33)
#define L_IS_B 36 // GPIO23 (D23)
// motor B pins end here

#define CW 1 //
#define CCW 0 //
#define debug 1 //


void setup() { 
  Serial.begin(115200);
  PS4.begin("40:99:22:d3:2e:ac");
  Serial.println("Ready.");

  // Buat PWM
  ledcSetup(RPWM_CHA, freq, resolution);
  ledcAttachPin(RPWM_A, RPWM_CHA);
  
  ledcSetup(LPWM_CHA, freq, resolution);
  ledcAttachPin(LPWM_A, LPWM_CHA);
  
  ledcSetup(RPWM_CHB, freq, resolution);
  ledcAttachPin(RPWM_B, RPWM_CHB);
  
  ledcSetup(LPWM_CHB, freq, resolution);
  ledcAttachPin(LPWM_B, LPWM_CHB);

  pinMode(R_EN_A, OUTPUT); // pin EN bisa dibuang (langsung hubung ke Vcc)
  digitalWrite(R_EN_A, HIGH); // biar motor A bisa CW
  pinMode(R_IS_A, INPUT_PULLUP); // ngasih tau berapa arus ngalir
  
  pinMode(L_EN_A, OUTPUT);
  digitalWrite(L_EN_A, HIGH); // biar motor A bisa CCW
  pinMode(L_IS_A, INPUT_PULLUP);  

  pinMode(R_EN_B, OUTPUT);
  digitalWrite(R_EN_B, HIGH); // biar motor B bisa CW
  pinMode(R_IS_B, INPUT_PULLUP);  
  
  pinMode(L_EN_B, OUTPUT);
  digitalWrite(L_EN_B, HIGH); // biar motor B bisa CCW
  pinMode(L_IS_B, INPUT_PULLUP);  
}

void loop() {
  if (PS4.isConnected()) {
    throttle = PS4.R2Value()-PS4.L2Value(); 
    
    if (throttle<49 and throttle>-50) { //buat ngarahin robot + yaw
      speedA = map(PS4.LStickX(), 0, 127, 0, MAXSPEEDA*1);
      speedB = map(-PS4.LStickX(), 0, 127, 0, MAXSPEEDB*1);
    }
    
    else { //buat mode setir biasa
      speedA = map(INITIALSPEED+PS4.LStickY()+PS4.LStickX()/2, 0, 200, 0, throttle);
      speedB = map(INITIALSPEED+PS4.LStickY()-PS4.LStickX()/2, 0, 200, 0, throttle);
  
      speedA = map(speedA, 0, 255, 0, MAXSPEEDA);
      speedB = map(speedB, 0, 255, 0, MAXSPEEDB);
  
      }
    }
  else{
  Serial.println("disconnected");
  delay(100);
  }
  
  if (speedA>20){ // motor A CW
    ledcWrite(LPWM_CHA, 0);
    ledcWrite(RPWM_CHA, speedA);
  }
  else if (speedA<-20){ // motor A CCW
    ledcWrite(RPWM_CHA, 0);
    ledcWrite(LPWM_CHA, -speedA);
  }
  else{ // motor A berenti
    ledcWrite(RPWM_CHA, 0);
    ledcWrite(LPWM_CHA, 0);
  }
  
  if (speedB>20){ // motor B CCW
    ledcWrite(RPWM_CHB, 0);
    ledcWrite(LPWM_CHB, speedB);
  }
  else if (speedB<-20){ // motor B CW
    ledcWrite(LPWM_CHB, 0);
    ledcWrite(RPWM_CHB, -speedB);
  }
  else{ // motor B berenti
    ledcWrite(RPWM_CHB, 0);
    ledcWrite(LPWM_CHB, 0);
  }
  
  if (debug==1){
    Serial.print("speedA: ");
    Serial.print(speedA);
    Serial.print(" speedB: ");
    Serial.println(speedB);
    Serial.println("");
    delay(100);
  }
  
}





  
