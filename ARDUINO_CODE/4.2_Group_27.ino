#include "html510.h"    
#include "4.2_Group_27.h"     // contains string "body" html code
HTML510Server h(80);
// one channel for each motor (PWM)
#define LEDC_CHANNEL1 0 // use first of 16  
#define LEDC_CHANNEL2 1 
#define LEDC_CHANNEL3 2 
#define LEDC_CHANNEL4 3 

#define LEDC_RESOLUTION_BITS 20 // up to 1M
#define LEDC_FREQ_HZ 20         // chosen frequency not stalling at high duty cycles
#define LEDC_RESOLUTION ((1<<LEDC_RESOLUTION_BITS)-1) 
// Motor 1
#define SPEED_PIN1 22
#define DIR_PIN1 19
#define NOT_DIR_PIN1 9
// Motor 2
#define SPEED_PIN2 10
#define DIR_PIN2 23
#define NOT_DIR_PIN2 18
// Motor 3
#define SPEED_PIN3 32
#define DIR_PIN3 26
#define NOT_DIR_PIN3 33
// Motor 4
#define SPEED_PIN4 4
#define DIR_PIN4 14
#define NOT_DIR_PIN4 27

const char* ssid = "ESP32 Sof";
int duty;

/*****************/
/* web handler   */
void handleRoot() {
  h.sendhtml(body);
}

// function triggered when we change the duty cycle
void handleSliderDuty(){
  String s2 = "Speed: ";
  duty = h.getVal();
  s2 = s2+ duty;
  h.sendplain(s2); 
}

// functions triggered when we press one of the buttons
void handleHit1(){            // forward left
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(NOT_DIR_PIN2,LOW);

    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(NOT_DIR_PIN3,LOW);

    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void handleHit2(){      // forward
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(NOT_DIR_PIN1,LOW); 

    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(NOT_DIR_PIN2,LOW);

    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(NOT_DIR_PIN3,LOW);

    digitalWrite(DIR_PIN4,HIGH);
    digitalWrite(NOT_DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);  
  h.sendplain(""); // acknowledge     
}

void handleHit3(){      // forward right
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(NOT_DIR_PIN1,LOW); 

    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(NOT_DIR_PIN2,LOW);

    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(NOT_DIR_PIN3,LOW);

    digitalWrite(DIR_PIN4,HIGH);
    digitalWrite(NOT_DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);  
  h.sendplain(""); // acknowledge     
}

void handleHit4(){      // left
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(NOT_DIR_PIN2,LOW);

    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(NOT_DIR_PIN3,LOW);

    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 
  h.sendplain(""); // acknowledge     
}

void handleHit5(){        // stop
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(NOT_DIR_PIN2,LOW);

    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(NOT_DIR_PIN3,LOW);

    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void handleHit6(){      // right
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(NOT_DIR_PIN1,LOW); 

    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(NOT_DIR_PIN2,HIGH);

    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(NOT_DIR_PIN3,HIGH);

    digitalWrite(DIR_PIN4,HIGH);
    digitalWrite(NOT_DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
  h.sendplain(""); // acknowledge     
}

void handleHit7(){      // backward left
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(NOT_DIR_PIN2,HIGH);

    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(NOT_DIR_PIN3,HIGH);

    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 
  h.sendplain(""); // acknowledge     
}
 
void handleHit8(){      // backwards
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(NOT_DIR_PIN2,HIGH); 

    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(NOT_DIR_PIN3,HIGH); 
    
    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 
  h.sendplain(""); // acknowledge     
}

void handleHit9(){      // backward right
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(NOT_DIR_PIN1,HIGH); 

    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(NOT_DIR_PIN2,HIGH); 

    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(NOT_DIR_PIN3,HIGH); 
    
    digitalWrite(DIR_PIN4,LOW);
    digitalWrite(NOT_DIR_PIN4,HIGH); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void setup() {  
  Serial.begin(115200);
  // Attach LEDC channels to motor speed pins
  ledcAttachPin(SPEED_PIN1, LEDC_CHANNEL1);
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN2, LEDC_CHANNEL2);
  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN3, LEDC_CHANNEL3);
  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN4, LEDC_CHANNEL4);
  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS); 
  
  // wifi configurations
  WiFi.softAP(ssid);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 136),  IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0)); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");  Serial.println(myIP);      
             
  h.begin();
  h.attachHandler("/ ",handleRoot);
  h.attachHandler("/slider?val2=",handleSliderDuty);
  h.attachHandler("/hit1",handleHit1);
  h.attachHandler("/hit2",handleHit2);
  h.attachHandler("/hit3",handleHit3);
  h.attachHandler("/hit4",handleHit4);
  h.attachHandler("/hit5",handleHit5);
  h.attachHandler("/hit6",handleHit6);
  h.attachHandler("/hit7",handleHit7);
  h.attachHandler("/hit8",handleHit8);
  h.attachHandler("/hit9",handleHit9);

  // make direction pins output pins
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(NOT_DIR_PIN1, OUTPUT);
  
  pinMode(DIR_PIN2, OUTPUT);
  pinMode(NOT_DIR_PIN2, OUTPUT);
  
  pinMode(DIR_PIN3, OUTPUT);
  pinMode(NOT_DIR_PIN3, OUTPUT);
  
  pinMode(DIR_PIN4, OUTPUT);
  pinMode(NOT_DIR_PIN4, OUTPUT); 
}

void loop(){
  h.serve();
}
