/* Heltec Automation I2C scanner example (also it's a basic example how to use I2C1)
 *
 * ESP32 have two I2C (I2C0 and I2C1) bus
 *
 * OLED is connected to I2C0, so if scan with Wire (I2C0), the return address should be 0x3C.
 *
 * If you need scan other device address in I2C1...
 *    - Comment all Wire.***() codes;
 *    - Uncomment all Wire1.***() codes;
 *
 * I2C scan example and I2C0
 *
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
 * */

#include "Arduino.h"
#include "heltec.h"

#if defined( Wireless_Stick_Lite )
  #include <Wire.h>
  #include "oled/SSD1306Wire.h"

  static const uint8_t SCL_OLED = 15;
  static const uint8_t SDA_OLED = 4;
#endif

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup()
{
  Heltec.begin(true, false, true);
  //Wire.begin(SDA_OLED, SCL_OLED); //Scan OLED's I2C address via I2C0
  Wire1.begin(SDA, SCL);        //If there have other device on I2C1, scan the device address via I2C1

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();

}

void loop()
{
//  byte error, address;
//  int nDevices;
//
//  Serial.println("Scanning...");
//
//  nDevices = 0;
//  for(address = 1; address < 127; address++ )
//  {
////    Wire.beginTransmission(address);
////    error = Wire.endTransmission();
//
//    Wire1.beginTransmission(address);
//    error = Wire1.endTransmission();
//
//    if (error == 0)
//    {
//      Serial.print("I2C device found at address 0x");
//      if (address<16)
//      Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
//
//      nDevices++;
//    }
//    else if (error==4)
//    {
//      Serial.print("Unknown error at address 0x");
//      if (address<16)
//        Serial.print("0");
//      Serial.println(address,HEX);
//    }
//  }
//  if (nDevices == 0)
//  Serial.println("No I2C devices found\n");
//  else
//  Serial.println("done\n");

//  delay(5000);
    Serial.print(sensor.readRangeContinuousMillimeters());
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Serial.println();
  
}