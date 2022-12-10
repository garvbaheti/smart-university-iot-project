/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLEzG0Eco2"
#define BLYNK_DEVICE_NAME "Project IOT MCU 5"
#define BLYNK_AUTH_TOKEN "FnEqTrh8QH8bsLhTJYTbQGJQO7U4CtHS"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Baheti";
//char pass[] = "P@SSW0RD123";
char ssid[] = "me.baheti.07";
char pass[] = "123456789";
int PUMP= 2;
int moisture = A0; // sensor input at Analog pin A0
int mois_value = 0;
int sys = 0;

//FOuntain System
int fnt_sys = 0;
const int IR1ledPin = 12; // choose pin for the LED
const int IR1inputPin = 13; // choose input pin (for Infrared sensor) 
const int IR2ledPin = 14;
const int IR2inputPin = 15;
int IR1val = 0; // variable for reading the pin status
int IR2val = 0;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger1 = 0;
boolean startTimer1 = false;

unsigned long lastTrigger2 = 0;
boolean startTimer2 = false;

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes 
BLYNK_WRITE(V2)
{
  // Set incoming value from pin V1 to a variable
  fnt_sys = param.asInt();
}
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(19200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  pinMode(PUMP, OUTPUT); // declare pump relay as output 
  pinMode(moisture, INPUT); // declare Infrared sensor as input
  pinMode(IR1ledPin, OUTPUT); // declare LED as output 
  pinMode(IR1inputPin, INPUT); // declare Infrared sensor as input
  pinMode(IR2ledPin, OUTPUT); // declare LED as output 
  pinMode(IR2inputPin, INPUT); // declare Infrared sensor as input
  attachInterrupt(digitalPinToInterrupt(IR1inputPin), detectsMovement1, RISING);
  attachInterrupt(digitalPinToInterrupt(IR2inputPin), detectsMovement2, RISING);
}
ICACHE_RAM_ATTR void detectsMovement1() {
  if(fnt_sys==1){
      Serial.println("MOTION DETECTED1!!!");
      digitalWrite(IR1ledPin, HIGH);
      startTimer1 = true;
      lastTrigger1 = millis();
      
   }
}
ICACHE_RAM_ATTR void detectsMovement2() {
  if(fnt_sys==1){
      Serial.println("MOTION DETECTED2!!!");
      digitalWrite(IR2ledPin, HIGH);
      startTimer2 = true;
      lastTrigger2 = millis();
      
   }
}
void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  mois_value = analogRead(A0);
  mois_value = map(mois_value, 0, 1023, 0, 100);
  Blynk.virtualWrite(V5, mois_value);
  if(fnt_sys==1){
      now = millis();
      if(startTimer1&&((now-lastTrigger1)>3000)){
        digitalWrite(IR1ledPin, LOW);
        startTimer1 = false;
      }
      now = millis();
      if(startTimer2&&((now-lastTrigger2)>3000)){
        digitalWrite(IR2ledPin, LOW);
        startTimer2 = false;
      }
   }
   else{
    digitalWrite(IR1ledPin, LOW);
    digitalWrite(IR2ledPin, LOW);
   }
  Serial.println(sys);
    if (mois_value > 50 ){
        digitalWrite(PUMP, HIGH); //
    }
    else{
    Serial.println("PUMP is OFF");
    digitalWrite(PUMP, LOW);
    }
}
