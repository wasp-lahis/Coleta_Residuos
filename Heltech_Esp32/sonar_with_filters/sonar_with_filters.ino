/* 
  Description: Filter on ultrassonic read on Heltech Wifi Lora 32
  By: Lahis Almeida 

  Based on:
   - https://www.newtoncbraga.com.br/index.php/microcontrolador/143-tecnologia/16326-moduloesp32-heltech-mec218
   - https://www.fernandok.com/2018/10/introducao-ao-esp32-wifi-lora.html
   - https://www.arduinoecia.com.br/calculador-divisor-de-tensao-function/ -> to use on echo pin
   - https://dzone.com/articles/internet-of-things-4-free-platforms-to-build-iot-p
  
  To run this code, select 'Heltech Wifi LoRa 32' on Tools>Board
 */

/* Includes para header files das biliotecas do OLED */
#include <Wire.h>
#include <Ultrasonic.h>

#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

#define NUM_AMOSTRAS 50
#define INTERVALO_LEITURA 100 //(ms)


/* ---------------------------- ULTRASSONIC SENSOR ---------------------------- */
int pin_echo= 22;
int pin_trigger = 23;

int aux = 0;
float raw_distance = 0; // armazenar a distância lida pelo sonar
float maverage_filter_distance = 0; // armazenar a distância lida pelo sonar

int maverage_vector[NUM_AMOSTRAS];
vector<float> distance_vector ;

Ultrasonic ultrasonic(pin_trigger, pin_echo);

 
void setup() {
    Serial.begin(115200);
}
 
void loop(){
   raw_distance = getDistance(); 
   maverage_filter_distance = moving_average(raw_distance);


//   Serial.print(raw_distance);
//   Serial.print(" ");
//   Serial.println(maverage_filter_distance);
   if (aux < NUM_AMOSTRAS){
      distance_vector.push_back(raw_distance);
      aux++;
      Serial.print(raw_distance);
      Serial.print(" ");
      Serial.println(maverage_filter_distance);
   }
   else{
//    Serial.println("\nDistancia medida - Filtro Media - Filtro de Mediana (cm)");
//    Serial.print(measure_distance);
//    Serial.print(",");
    Serial.print(raw_distance);
    Serial.print(" ");
    Serial.print(maverage_filter_distance);
    Serial.print(" ");
    Serial.print(mean(distance_vector));
    Serial.print(" ");
    Serial.print(median(distance_vector));
    Serial.println();    
    
    distance_vector.clear();
    aux = 0;
   }
   
   delay(INTERVALO_LEITURA);
}


float getDistance(){    
    float distanciaCM;
    long microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
    return distanciaCM;
}


// ============== FILTERS FUNCTIONS ==============
float mean(vector<float> &distance){
  float sum = 0.0;
  for(int i = 0; i < distance.size(); i++){
    sum += distance[i];
  }
  return (sum/distance.size());
}

float median(vector<float> &distance){
  sort(distance.begin(), distance.end());

  if(distance.size() % 2 == 0){// se par
    return ( (distance[distance.size()/2] + distance[distance.size()/2 -1]) / 2);
  }
  else{ // impar
    return (distance[floor(distance.size()/2)]); // round down
  }
}


float moving_average(float raw_distance){
  // desloca os elementos do vetor de média móvel
  for (int i = NUM_AMOSTRAS-1; i > 0; i--) maverage_vector[i] = maverage_vector[i-1];

  maverage_vector[0] = raw_distance;

  long somatorio = 0;

  for (int i = 0; i < NUM_AMOSTRAS; i++) somatorio += maverage_vector[i]; 

   return somatorio/NUM_AMOSTRAS;
}
