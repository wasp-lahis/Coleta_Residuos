/* 
  Description: 
  - Bin Level program 01
  - includes: ultrassonic read, mqtt pub, integration with Ubidots and sleep mode
  - not included: distance filter
  
  By: Lahis Almeida 
  Based on:
   - https://www.fernandok.com/2018/11/introducao-ao-protocolo-mqtt-com.html
  
  To run this code, select 'Heltech Wifi LoRa 32' on Tools>Board
 */

/* Includes para header files das biliotecas do OLED */
#include <Ultrasonic.h>
#include "MQTT_Client.h" //Arquivo com as funções de mqtt

#include "heltec.h" // deep sleep

/* ---------------------------- DEEP SLEEP ---------------------------- */

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


/* ---------------------------- ULTRASSONIC SENSOR ---------------------------- */

#define INTERVALO_LEITURA 250 //(ms)

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


// ---------- Deep Sleep Functions ----------
// Method to print the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  :
      Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  :
      Serial.println("Wakeup caused by timer"); break;
    case 4  :
      Serial.println("Wakeup caused by touchpad"); break;
    case 5  :
      Serial.println("Wakeup caused by ULP program"); break;
    default :
      Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
 
void setup() {
  delay(2000);
  Serial.begin(115200);

  // Deep Sleep
  Heltec.begin(false, false, true /*Serial Enable*/, false /*PABOOST Enable*/);
   ++bootCount;
  Serial.println("\n\t\t !! Wake UP !! ");
  Serial.println("Boot number: " + String(bootCount));
  delay(2);
  
  print_wakeup_reason(); // Print the wakeup reason for ESP32
  
  Serial.println("Setting up mqtt...");
  //Inicializa mqtt (conecta o esp com o wifi, configura e conecta com o servidor da ubidots)
  if(!mqttInit()){        
    delay(3000);
    Serial.println("MQTT CONECTION Failed!");
    ESP.restart();
  }
  else{
    
    Serial.println("MQTT CONECTION OK");  
    distance = getDistance(); 
  
    Serial.print("\nDistancia atual: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    if(sendValues(distance)){ Serial.println("Successfully sent data"); }
    else{                          Serial.println("Failed to send sensor data");}
       
    delay(1000);

    
    // First we configure the wake up source. We set our ESP32 to wake up every X seconds
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +  " Seconds");
    delay(10);   
    /* Now that we have setup a wake cause and if needed setup the peripherals state in deep sleep, we can now start going to deep sleep.
       In the case that no wake up sources were provided but deep sleep was started, it will sleep forever unless hardware reset occurs.
    */
    Serial.println("Going to sleep now in 5 seconds");
    delay(5000);
    esp_deep_sleep_start();
    
   }

  
}
 
void loop(){
  // Se o esp foi desconectado do ubidots, tentamos reconectar
//  if(!heltechClient.connected())
//    reconnect();
 
}
