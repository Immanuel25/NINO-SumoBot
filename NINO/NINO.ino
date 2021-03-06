#include <PS4Controller.h>

// FITUR YANG KURANG
// 1. Sirine dan lampu ambulans
// 2. Baca arus dari bts7960
// 3. COMBO MOVE (LMAO)

int MAXSPEEDA = 800; // 800/1023 x100% duty cycle 
int MAXSPEEDB = 800; // 800/1023 x100% duty cycle
int INITIALSPEED = 64; 

int speedA = 0;
int speedB = 0;
int throttle = 0;

const int freq = 5000;
const int resolution = 10; //Resolution 8, 10, 12, 15

bool xButtonState = false;
bool alarmState = false;
int waktuNi = 0;
int ganjilGenap = 0;


#define pinMerah 33 // GPIO23 (D23)
#define pinBiru 32 // GPIO22 (D23)
#define pinBuzzer 18 // GPIO18 (D18)

// pins for motor A
#define RPWM_CHA 0 // CHANNEL PWM FORWARD MOTOR A
#define RPWM_A 17 // GPIO17 (D17)
//#define R_IS_A 15 // GPIO15 (D15)

#define LPWM_CHA 1 // CHANNEL PWM REVERSE MOTOR A
#define LPWM_A 16 // GPIO16 (D16)
//#define L_IS_A 2 // GPIO2 (D2)
// motor A pins end here

// pins for motor B
#define RPWM_CHB 2 // CHANNEL PWM FORWARD MOTOR B
#define RPWM_B 26 // GPIO26 (D26)
//#define R_IS_B 12 // GPIO12 (D12)

#define LPWM_CHB 3 // CHANNEL PWM REVERSE MOTOR B
#define LPWM_B 27 // GPIO27 (D27)
//#define L_IS_B 14 // GPIO14 (D14)
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

//  pinMode(R_IS_A, INPUT); // ngasih tau berapa arus ngalir
//  pinMode(L_IS_A, INPUT); 
//  pinMode(R_IS_B, INPUT);  
//  pinMode(L_IS_B, INPUT);  

  pinMode(pinMerah, OUTPUT);
  pinMode(pinBiru, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  ledcAttachPin(pinBuzzer, 4);
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
 
    if (PS4.Cross() and xButtonState == false) {
      if (alarmState == true){ //matiin alarm
        digitalWrite(pinMerah, LOW);
        digitalWrite(pinBiru, LOW);
        ledcWrite(4, 0);
        alarmState = false;
      }
      else {
        alarmState = true;
        waktuNi = millis();
      }
    }
    
    xButtonState = PS4.Cross();
  
    if (alarmState == true){
      ganjilGenap = (millis() - waktuNi)/500;
      ganjilGenap = ganjilGenap%2;
      
      if (ganjilGenap == 0){
        digitalWrite(pinMerah, LOW);
        digitalWrite(pinBiru, HIGH);
        ledcWriteTone(4, 50);
      }
      else{
        digitalWrite(pinMerah, HIGH);
        digitalWrite(pinBiru, LOW);
        ledcWriteTone(4, 500);
      }
    }
  }
  else{
    Serial.println("disconnected");
    delay(100);
  }
}





  
