/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLbUcwQSJO"
#define BLYNK_DEVICE_NAME "Temperatur"
#define BLYNK_AUTH_TOKEN "BfBlcH1rUKJgl2o8bEH_mpBuZf7pQKqf"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial



#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 0


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


Adafruit_BMP280 bme; // I2C


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "dein wlan name";
char pass[] = "dein Wlan passwort";

uint8_t LED_R = 0;
uint8_t LED_G = 0;
uint8_t LED_B = 0;
int pinValue = 0;
bool tempRGB = false;
bool tempFeed = false;
double tempBMP = 0;
CRGB leds[NUM_LEDS];

BlynkTimer timer;
BlynkTimer timer_Temp;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  pinValue = param.asInt();
  if (pinValue == 1){
    tempRGB = true;
  }else{
    tempRGB = false;
      fill_solid(leds, NUM_LEDS, CRGB(LED_R, LED_G, LED_B));
      FastLED.show();
  }
  // Update state
  Blynk.virtualWrite(V1, pinValue);
  Serial.println("Test");
}



BLYNK_WRITE(V3)
{
  LED_R = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.println("V3 Slider value is: ");
  
  Serial.println(LED_R);
  Serial.println(LED_G);
  Serial.println(LED_B);
  fill_solid(leds, NUM_LEDS, CRGB(LED_R, LED_G, LED_B));
  FastLED.show();
}


BLYNK_WRITE(V4)
{
  LED_G = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.println("V4 Slider value is: ");
  
  Serial.println(LED_R);
  Serial.println(LED_G);
  Serial.println(LED_B);
  fill_solid(leds, NUM_LEDS, CRGB(LED_R, LED_G, LED_B));
  FastLED.show();
}


BLYNK_WRITE(V5)
{
  LED_B = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.println("V5 Slider value is: ");
  
  Serial.println(LED_R);
  Serial.println(LED_G);
  Serial.println(LED_B);
  fill_solid(leds, NUM_LEDS, CRGB(LED_R, LED_G, LED_B));
  FastLED.show();
}


BLYNK_WRITE(V7){
  int pinV7 = param.asInt();
  if (pinV7){
    tempFeed = true;
  }else{
    tempFeed = false;
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));  
    FastLED.show();
  }
}

void tempShow(){
  double tempRef = tempBMP;
  int tempR;
  int tempG;
  int tempB;
  if (tempRef < 19){
    tempRef = tempBMP;
    if (tempRef < 16) tempRef = 16;
    tempB = map(tempRef, 16, 19, 255, 0);
    tempG = map(tempRef, 16, 19, 0, 255);
    fill_solid(leds, NUM_LEDS, CRGB(0, tempG, tempB));
    FastLED.show();
  }
  if (tempRef >= 19 && tempRef < 21){
    fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
    FastLED.show();
  }
  if (tempRef >= 21){
    tempRef = tempBMP;
    if (tempRef > 23) tempRef = 23;
    tempR = map(tempRef, 21, 23, 0, 255);
    tempG = map(tempRef, 21, 23, 255, 0);
    fill_solid(leds, NUM_LEDS, CRGB(tempR, tempG, 0));
    FastLED.show();
  }




}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{

}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
void sendTemp()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  tempBMP = bme.readTemperature();
  Blynk.virtualWrite(V6, tempBMP);
}
void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  timer_Temp.setInterval(10000L, sendTemp);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  if (!bme.begin(0x76)) { 
  Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  while (1);
  }
  for (int i= 0; i < 20; i++){
      fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  delay(100);
    fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
  FastLED.show();
  delay(100);
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255));
  FastLED.show();
  delay(100);
  }
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();
}

void loop()
{
  Blynk.run();
  timer.run();
  timer_Temp.run();
  if (tempRGB){
    fill_rainbow(leds, NUM_LEDS, beat8(10,255), 5);            
    FastLED.show();
  }
  if (tempFeed){
    tempShow();
  }
  //FastLED.show();
  //Serial.println(LED_R);
  //Serial.println(LED_G);
  //Serial.println(LED_B);
  //Serial.println("                ");
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
