// Made by Hannah van Iterson (2018), 2nd year Bachelor student at the Department of Industrial Design at the TU/e.  
// This code was inspired by :
// https://learn.adafruit.com/adafruit-am2320-temperature-humidity-i2c-sensor/arduino-usage. 
// The standard libaries from Adafruit_AM2320. (The part of this code that read data from the sensor was copied from the libary Adafruit_AM2320 library)
// 
// This code was revised and created with help of: 
// Bas van Rossem, 2e jaar HBO ICT te Utrecht, Richting: TI (Technische Informatica)

#include "Adafruit_AM2320.h"

//One cannot change const values while the program is running.
//This can only be done while editing the program in the IDE.
const int second = 1000;
const float tempChange = 1.0; //Required temparture change needed to start flow of electricity current. 
const int TemplaagPin = 5;
const int VervormPin = 9;

bool Geblazen = false;
float Tempstart;

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setup() {
  Serial.begin(9600);
  pinMode(TemplaagPin, OUTPUT);
  pinMode(VervormPin, OUTPUT);

  //Start the reading of the sensor
  Serial.println("Adafruit AM2320 Basic Test");
  am2320.begin();

  //Safe the first temperature.
  Tempstart = am2320.readTemperature();

  //Wait for the port to open
  while (!Serial) {
    delay(100);
  }

  Serial.println("Setup completed");
}

void loop() {
  //Print the temperature
  Serial.print("Temp: ");
  Serial.println(am2320.readTemperature() );
  tempLed ();
  Serial.print("Original:  ");
  Serial.println( Tempstart);
  delay(100);
  count();
}

void keyPress(){
  if (Serial.available() > 0) {
    if (char(Serial.read()) == 'r') {
      Serial.println("Reset start temperature");
      Tempstart = am2320.readTemperature();
    }
  }
}

void tempLed() {
  //Find the difference between the current temp and that of the atmosphere.
  float Tempnow = am2320.readTemperature();
  float Change = (Tempstart + tempChange);

  //Check the current temperature to the original temperature.
  if (Tempnow < Change) {
    digitalWrite (TemplaagPin, HIGH);
    Geblazen = false;
    Serial.println(Geblazen);
  }
  else {
    digitalWrite (TemplaagPin, LOW);
    Geblazen = true;
    Serial.println(Geblazen);
  }
}

void count() {
  if (Geblazen) {
    //Start current flow
    digitalWrite (VervormPin, HIGH);
    Serial.println("Current started for 15 seconds");
    for(int i = 0; i < 180; i++){
      keyPress();
      delay (second / 10);
    }
    
    Geblazen = false;
    
    //Stop current flow
    digitalWrite (VervormPin, LOW);
    Serial.println("Current stopped");
  }
}
