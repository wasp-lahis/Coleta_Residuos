/* 
  Description: Heltech reads values from sonar and lidar sensors
  By: Lahis Almeida 

  Based on:
   - https://www.newtoncbraga.com.br/index.php/microcontrolador/143-tecnologia/16326-moduloesp32-heltech-mec218
   - https://www.fernandok.com/2018/10/introducao-ao-esp32-wifi-lora.html
   - https://www.arduinoecia.com.br/calculador-divisor-de-tensao-function/ -> to use on echo pin
   - https://dzone.com/articles/internet-of-things-4-free-platforms-to-build-iot-p

   - https://github.com/adafruit/Adafruit_VL53L0X
  
  To run this code, select 'Heltech Wifi LoRa 32' on Tools>Board
 */
 
// --- Sonar libs ---
#include <Wire.h>
#include <Ultrasonic.h>

#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

#define NUM_AMOSTRAS 20
#define INTERVALO_MS_LEITURA 100 


// --- Lidar lib ---
#include "Adafruit_VL53L0X.h"


// --- Sonar variables ---

byte pin_echo = 13; // byte - stores an 8-bit unsigned number, from 0 to 255.
byte pin_trigger = 12;

int aux = 0;
float sonar_raw_distance = 0.0; // armazenar a distância lida pelo sonar
vector<float> distance_vector ;

Ultrasonic ultrasonic(pin_trigger, pin_echo);


// --- Lidar variables ---

// I2C BUS 1 (default):
// - SDA PIN: 21
// - SCL PIN: 22

Adafruit_VL53L0X lidar_lox = Adafruit_VL53L0X();

float lidar_raw_distance = 0.0; // armazenar a distância lida pelo lidar


// ============== Ultrassonic FUNCTIONS ==============

float sonar_get_distance(){    
    float distanciaCM;
    long microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
    return distanciaCM;
}

float sonar_get_mean_value(vector<float> &distance){
  float sum = 0.0;
  for(int i = 0; i < distance.size(); i++){
    sum += distance[i];
  }
  return (sum/distance.size());
}

float sonar_get_median_value(vector<float> &distance){
  sort(distance.begin(), distance.end());

  if(distance.size() % 2 == 0){// se par
    return ( (distance[distance.size()/2] + distance[distance.size()/2 -1]) / 2);
  }
  else{ // impar
    return (distance[floor(distance.size()/2)]); // round down
  }
}



// ============== Lidar FUNCTIONS ==============

void lidar_init(){

  if (!lidar_lox.begin()) {
      while(1){
       Serial.println(F("Failed to boot VL53L0X"));  
       delay(10); // ms
      };
  }
}

float lidar_get_distance(){
  
  VL53L0X_RangingMeasurementData_t measure;
    
  // Reading a measurement... 
  lidar_lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    return (measure.RangeMilliMeter/10.0); // mm to cm 
  }
  return 0.0;
}


// ============== MAIN CODE ==============
void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  
  lidar_init();
}


void loop() {

   sonar_raw_distance = sonar_get_distance(); 
   lidar_raw_distance = lidar_get_distance();
   
   if (aux < NUM_AMOSTRAS){
      distance_vector.push_back(sonar_raw_distance);
      aux++;
   }
   else{
    Serial.print(sonar_raw_distance);
    Serial.print(" ");
    Serial.print(sonar_get_mean_value(distance_vector));
    Serial.print(" ");
    Serial.print(sonar_get_median_value(distance_vector));
    Serial.print(" ");
    Serial.print(lidar_raw_distance);
    Serial.println();    
    
    distance_vector.clear();
    aux = 0;
   }


 
  delay(INTERVALO_MS_LEITURA);
}
