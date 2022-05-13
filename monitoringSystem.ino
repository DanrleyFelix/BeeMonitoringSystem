
/*
   Smart remote bee monitoring system - Data acquisition
   April 4, 2022
   Federal University of Paraíba
   Author: Danrley Santos Felix
   github: https://github.com/DanrleyFelix/
   Install Adafruit Unified Sensor
   ===============================================================================
   Module light sensor LDR -> PIN DO: Digital output
   Turning it clockwise will increase the sensitivity and increase the detection range
   ===============================================================================
   DHT11 (RAW Sensor) -> PIN 1: 5V - PIN 2: SIGNAL - PIN 3: NO CONNECTION - PIN 4: GND
   ===============================================================================
   BME280 -> PIN 1: 3.3V or 5V - PIN 2: GND - PIN 3: SCL - PIN 4: SDA (I2C)
   Address: 0x76
   ===============================================================================
   Module sound sensor HW-484 -> PIN AO: Analog output - PIN V+: VCC - PIN G: GND
   Rotate the knob counterclockwise to increase sensitivity
*/

// ===============================================================================
// --- Including libraries ---
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <HX711.h>
#include <Wire.h>

// ===============================================================================
// --- Hardware mapping - Arduino pro micro - ATMEGA32U4 Architecture
#define LDRPIN PD7
#define DHTPIN 9
#define HWPIN A8
#define DHTTYPE DHT11 

// ===============================================================================
// --- Constants ---
#define N 20
#define BME_I2C_ADDRESS 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

// ===============================================================================
// --- Prototypes ---
float avg(float *vector, int n);
void sensorDHT();
void sensorLDR();
void sensorBME();
void sensorHW();
void sensorHX();
void sensorACS();

// ===============================================================================
// --- Objects  ---
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BME280 bme;

// ===============================================================================
// --- Variables  ---
float humidity[N], temperature[N], sound[N], intHumidity[N], intTemperature[N], intPressure[N];
float avgHumidity, avgTemperature, avgSound, avgIntHumidity, avgIntTemperature, avgIntPressure;
int light;
unsigned long int previousTime = 0;
String data = "<"; 

void setup(){

  bool status;
  Serial.begin(9600);
  status = bme.begin(BME_I2C_ADDRESS);
  if (!status){
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (true);
  }
  pinMode(LDRPIN, INPUT);
  pinMode(HWPIN, INPUT);
  dht.begin();
  delay(5000);
  previousTime = millis();
  
}

void loop(){

  if (millis() - previousTime >= 4000){
    sensorLDR();
    sensorDHT();
    sensorBME();
    sensorHW();
  }
  if (millis() - previousTime >= 5000){
    data.concat(light);
    data.concat(" ; ");
    data.concat(avgHumidity);
    data.concat(" ; ");
    data.concat(avgTemperature);
    data.concat(" ; ");
    data.concat(avgSound);
    data.concat(" ; ");
    data.concat(avgIntHumidity);
    data.concat(" ; ");
    data.concat(avgIntTemperature);
    data.concat(" ; ");
    data.concat(avgIntPressure);
    data.concat(">");
    Serial.println(data);
    data = "<";
    previousTime = millis();
  }

}

void sensorLDR(){

    int testLDR;
    testLDR = (!(digitalRead(LDRPIN)));
    if (!(isnan(testLDR))) light = testLDR;
    
}

void sensorDHT(){

    static unsigned int k = 0;
    float hum, temp;
    
    hum = 0.9341*dht.readHumidity() + 22.02;
    temp = 0.9741*dht.readTemperature() + 4.2;

    if (k >= N){
      avgHumidity = avg(humidity, k);
      avgTemperature = avg(temperature, k);
      k = 0;
    }
    if (!(isnan(temp) || isnan(hum))){
      humidity[k] = hum;
      temperature[k] = temp;
      k++;
    }
}

void sensorBME(){

    static unsigned int k = 0;
    float hum, temp, pres;

    temp = bme.readTemperature();
    pres = bme.readPressure();
    hum = bme.readHumidity();

    if (k >= N){
      avgIntHumidity = avg(intHumidity, k);
      avgIntTemperature = avg(intTemperature, k);
      avgIntPressure = avg(intPressure, k);
      k = 0;
    }
    if (!(isnan(temp) || isnan(hum) || isnan(pres))){
      intHumidity[k] = hum;
      intPressure[k] = pres;
      intTemperature[k] = temp;
      k++;
    }
}

void sensorHW(){

    static unsigned int k = 0;
    float testHW;
    testHW = analogRead(HWPIN) * 5.0 / 1023.0;
    if (k >= N){
      avgSound = avg(sound, k);
      k = 0;
    }
    if (!(isnan(testHW))){
      sound[k] = testHW;
      k++;
    }

}

float avg(float *vector, int n){

    float sum = 0;
    for(int i = 0; i < n; i++)
      sum += vector[i];
    return sum/n;

}
