/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <Wire.h>
#include "rgb_lcd.h"


#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"

const int pinAdc = A1;



const int colorR = 0;
const int colorG = 250;
const int colorB = 0;
const int pinLight = A0;
const int pinSound = A1;
const int pinTemperature = A2;
float light=0.0;
float sound;

int a; //thermistor value
float c;
int b = 3985;      //b value of the thermistor
float resistance;

rgb_lcd lcd;


// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xAD, 0xBE, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 10, 1);
IPAddress server(192, 168, 10, 10);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(light);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","Sensor Info");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{


   //Serial.begin(115200);
    //Serial.println("Grove - Sound Sensor Test...");



    
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);
  Serial.begin(57600);



printf("Light = %f", light);
  client.setServer(server, 1883);
  pinMode(A2, INPUT);
  client.setCallback(callback);
  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.print("Welcome to IOTLab");
    delay(1500);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
   int value = analogRead(A0);
  value = map(value, 0, 800, 0, 10);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Value: ");
  lcd.setCursor(0,1);
  lcd.print(value);
  delay(1000);

  //int f = (c*(9/5)+32);//convert to F (delete if you want C)
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("temperature is:");
  lcd.setCursor(0,1);
  //lcd.print("f");//change to lcd.print("c"); if you want C
  delay(1000);//wait 1 seco

   int sensorValueLight = analogRead(pinLight);
   int sensorValueSound = analogRead(pinSound);
   int sensorValueTemperature = analogRead(pinTemperature);
   a = sensorValueTemperature; //find the original reading of the thermistor
   resistance = (float)(1023 - a) * 10000 / a; //get the resistance of the sensor;
   c = 1 / (log(resistance / 10000) / b + 1 / 298.15) - 273.15; //convert to Celsius via datasheet&nbsp;;
   sensorValueTemperature = c;

   //float lux = exp(float(sensorValueLight)/20.0);
   //sensorValueLight = int(lux);

    String light, sound;
    if (sensorValueLight < 10) {
      light = "Dark";
  } 
  else if (sensorValueLight < 200) {
    light = "Dim";
  } 
  else if (sensorValueLight < 400) {
    light = "Light";
  } 
  else if (sensorValueLight < 700) {
    light = "Bright";
  } else {
    light = "Very bright";
  }

   long sum = 0;
    for(int i=0; i<32; i++){
        sum += analogRead(pinAdc);
    }

    sum >>= 5;

    delay(5000);
    char copy[15];
    String sensorLight = String(sensorValueLight);
    sensorLight.toCharArray(copy, 15);
    client.publish("Light", copy);

    if (sensorValueSound < 10) {
      sound = "Quiet";
  } 
  else if (sensorValueSound < 200) {
    sound = "Moderate";
  } 
  else if (sensorValueSound < 450) {
    sound = "Noisy";
  } 
  else {
    sound = "Intolerable";
  }

    String sensorSound = String(sensorValueSound);
    sensorSound.toCharArray(copy, 15);
    client.publish("Sound", copy);
    //printf("%s\n", sensorValue);

    String sensorTemperature = String(sensorValueTemperature);
    sensorTemperature.toCharArray(copy, 15);
    client.publish("Temperature", copy);

    int humidity = random(40, 100);
    String Humid = String(humidity);
    Humid.toCharArray(copy, 15);
    client.publish("Humidity", copy);
        
    Serial.print("light: ");
    Serial.println(sensorValueLight);
    Serial.print("sound: ");
    Serial.println(sensorValueSound);
    Serial.print("temperature: ");
    Serial.println(sensorValueTemperature);
    Serial.print("Humidity: ");
    Serial.println(Humid);
    }

   
