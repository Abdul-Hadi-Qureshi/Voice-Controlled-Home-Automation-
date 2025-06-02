#include <DHT.h>

#define BLYNK_TEMPLATE_ID "TMPL6-K-GmFMM"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "dwJbBKJ1DVsk-axpwi43301q8xvbd5og"

char ssid[] = "Abdul Hadi";
char pass[] = "11223344";

bool fetch_blynk_state = true;  //true or false

//Update the HEX code of IR Remote buttons 0x<HEX CODE>
#define IR_Button_1   0x10EF9E61
#define IR_Button_2   0x10EFEE11
#define IR_Button_3   0x10EFDE21
#define IR_Button_4   0x10EF2ED1
#define IR_All_Off    0x10EF1EE1

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>  
#include <IRremote.h>

#define DHTPIN              16 //D16 pin
#define IR_RECV_PIN         35 // D35 pin

#define DHTTYPE DHT11     // DHT 11

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19

#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27

#define wifiLed   2   //D2

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

#define VPIN_BUTTON_C    V5
#define VPIN_TEMPERATURE V6
#define VPIN_HUMIDITY    V7

// Relay State
bool toggleState_1 = LOW; 
bool toggleState_2 = LOW; 
bool toggleState_3 = LOW; 
bool toggleState_4 = LOW; 

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;

int wifiFlag = 0;
float temperature1 = 0;
float humidity1   = 0;

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);


BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
}

BLYNK_WRITE(VPIN_BUTTON_C) {
  all_SwitchOff();
}

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, HIGH); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, HIGH); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, HIGH); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); delay(100);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
}

void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    }
    digitalWrite(wifiLed, HIGH);
  
  }
}

BLYNK_CONNECTED() {
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
    Blynk.syncVirtual(VPIN_TEMPERATURE);
    Blynk.syncVirtual(VPIN_HUMIDITY);  
}

void readSensor(){
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT");
    return;
  }
  else {
    humidity1 = h;
    temperature1 = t;
  }  
}

void sendSensor()
{
  readSensor();
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
}

void ir_remote(){
  if (irrecv.decode(&results)) {
      switch(results.value){
          case IR_Button_1:  
            digitalWrite(RelayPin1, toggleState_1);
            toggleState_1 = !toggleState_1;
            Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
            delay(100);            
            break;
          case IR_Button_2:  
            digitalWrite(RelayPin2, toggleState_2);
            toggleState_2 = !toggleState_2;
            Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
            delay(100);            
            break;
          case IR_Button_3:  
            digitalWrite(RelayPin3, toggleState_3);
            toggleState_3 = !toggleState_3;
            Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
            delay(100);            
            break;
          case IR_Button_4:  
            digitalWrite(RelayPin4, toggleState_4);
            toggleState_4 = !toggleState_4;
            Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
            delay(100);            
            break;
          case IR_All_Off:
            all_SwitchOff();  
            break;
          default : break;         
        }   
        irrecv.resume();   
  } 
}

void manual_control()
{
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW) {
    digitalWrite(RelayPin1, LOW);
    toggleState_1 = HIGH;
    SwitchState_1 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Serial.println("Switch-1 on");
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH) {
    digitalWrite(RelayPin1, HIGH);
    toggleState_1 = LOW;
    SwitchState_1 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Serial.println("Switch-1 off");
  }
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW) {
    digitalWrite(RelayPin2, LOW);
    toggleState_2 = HIGH;
    SwitchState_2 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Serial.println("Switch-2 on");
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH) {
    digitalWrite(RelayPin2, HIGH);
    toggleState_2 = LOW;
    SwitchState_2 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Serial.println("Switch-2 off");
  }
  if (digitalRead(SwitchPin3) == LOW && SwitchState_3 == LOW) {
    digitalWrite(RelayPin3, LOW);
    toggleState_3 = HIGH;
    SwitchState_3 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Serial.println("Switch-3 on");
  }
  if (digitalRead(SwitchPin3) == HIGH && SwitchState_3 == HIGH) {
    digitalWrite(RelayPin3, HIGH);
    toggleState_3 = LOW;
    SwitchState_3 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Serial.println("Switch-3 off");
  }
  if (digitalRead(SwitchPin4) == LOW && SwitchState_4 == LOW) {
    digitalWrite(RelayPin4, LOW);
    toggleState_4 = HIGH;
    SwitchState_4 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    Serial.println("Switch-4 on");
  }
  if (digitalRead(SwitchPin4) == HIGH && SwitchState_4 == HIGH) {
    digitalWrite(RelayPin4, HIGH);
    toggleState_4 = LOW;
    SwitchState_4 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    Serial.println("Switch-4 off");
  }
}  

void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  irrecv.enableIRIn(); 
  dht.begin();    

  digitalWrite(wifiLed, LOW);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus);
  timer.setInterval(1000L, sendSensor); 
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  }
}

void loop()
{ 
  manual_control();
  ir_remote(); 
  Blynk.run();
  timer.run();
}
