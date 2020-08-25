/* 
  Description: ultrassonic read, mqtt pub and integration with Ubidots
  By: Lahis Almeida 

  Based on:
   - https://dzone.com/articles/internet-of-things-4-free-platforms-to-build-iot-p
   - https://www.fernandok.com/2018/11/introducao-ao-protocolo-mqtt-com.html
  
  To run this code, select 'Heltech Wifi LoRa 32' on Tools>Board
 */

/* Includes para header files das biliotecas do OLED */
#include <Wire.h>
#include <Ultrasonic.h>
#include "MQTT_Client.h" //Arquivo com as funções de mqtt

#define INTERVALO_LEITURA 250 //(ms)

/* ---------------------------- ULTRASSONIC SENSOR ---------------------------- */
int pin_echo= 22;
int pin_trigger = 23;

float distance = 0; // armazenar a distância lida pelo sonar

Ultrasonic ultrasonic(pin_trigger, pin_echo);


// ---------- Ultrassonic Functions ----------
float getDistance(){    
    float distanciaCM;
    long microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
    return distanciaCM;
}

 
void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("Setting up mqtt...");

  //Inicializa mqtt (conecta o esp com o wifi, configura e conecta com o servidor da ubidots)
  if(!mqttInit()){        
    delay(3000);
    Serial.println("Failed!");
    ESP.restart();
  }
  Serial.println("OK");  
}
 
void loop(){
  // Se o esp foi desconectado do ubidots, tentamos reconectar
  if(!heltechClient.connected())
    reconnect();

  distance = getDistance(); 
  
  Serial.print("\nDistancia atual: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if(sendValues(distance)){ Serial.println("Successfully sent data"); }
  else{                          Serial.println("Failed to send sensor data");}
     
  delay(1000);
}
