/* 
  Description: Ultrassonic basic read test
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

#define INTERVALO_LEITURA 250 //(ms)

/* ---------------------------- ULTRASSONIC SENSOR ---------------------------- */
int pin_echo= 22;
int pin_trigger = 23;

float distancia = 0; // armazenar a distância lida pelo sonar

Ultrasonic ultrasonic(pin_trigger, pin_echo);

 
void setup() {
    Serial.begin(115200);
}
 
void loop(){
   
   verificarDistancia();
   delay(1000);
}


float getDistance(){    
    float distanciaCM;
    long microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
    return distanciaCM;
}


void verificarDistancia(){
   
  distancia = getDistance(); 
  
  Serial.print("\nDistancia atual: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
//  if( int(distancia) <= 10 ){
//    Serial.println("\nDistancia minima atingida! ");
//  }
}
