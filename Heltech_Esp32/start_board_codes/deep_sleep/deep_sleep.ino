/*
 * Description: Deep Sleep on Heltech Wifi Lora 32
 * By: Lahis Almeida 
 * 
 * Function summary:
 * - Vext connected to 3.3V via a MOS-FET, the gate pin connected to GPIO21;
 * - OLED display and PE4259(RF switch) use Vext as power supply;
 * - WIFI Kit series V1 don't have Vext control function;
 * - Basic LoRa Function;
 * - Esp_Deep_Sleep Function;
 * 
 * Based on:
 * https://github.com/HelTecAutomation/Heltec_ESP32
 * https://github.com/HelTecAutomation/Heltec_ESP32/blob/master/src/heltec.h
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html
 * https://github.com/HelTecAutomation/Heltec_ESP32/blob/master/src/lora/LoRa.cpp
 * https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/ -> sleep modes
 * 
 * 
 * To run this code, select 'Heltech Wifi LoRa 32' on Tools>Board
*/

#include "heltec.h"
 
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


// Method to print the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case 1  :
      Serial.println("Wakeup caused by external signal using RTC_IO"); break;
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

void setup(){
  delay(200);
  
  // Lib Default: void begin(bool DisplayEnable=true, bool LoRaEnable=true, bool SerialEnable=true, bool PABOOST=true, long BAND=470E6);
  Heltec.begin(false, false, true /*Serial Enable*/, false /*PABOOST Enable*/);
  
  ++bootCount;
  Serial.println("\n\t\t !! Wake UP !! ");
  Serial.println("Boot number: " + String(bootCount));
  delay(2);

  // Print the wakeup reason for ESP32
  print_wakeup_reason();

  // First we configure the wake up source. We set our ESP32 to wake up every X seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +  " Seconds");
  delay(10);

  /*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals not needed by the wakeup source
  If you want to be a poweruser read in detail at the API docs:
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  
  /* Now that we have setup a wake cause and if needed setup the peripherals state in deep sleep, we can now start going to deep sleep.
     In the case that no wake up sources were provided but deep sleep was started, it will sleep forever unless hardware reset occurs.
  */
  Serial.println("Going to sleep now");
  delay(2);
  esp_deep_sleep_start();
  
  Serial.println("This will never be printed");
}

void loop(){} // This is not going to be called 
