#define MAX 50

#include <BluetoothSerial.h>
#include <Wire.h>
#include "FallDetection.h"
#include "HeartRate.h"

BluetoothSerial SerialBT;
FallDetection mpu;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  Wire.begin();
  mpu.init();

}

void loop() {

  mpu.process();
  printdata(mpu.data);
}

void printdata(String*info){
  for (int i=0; i < MAX; i++){
    if (info[i] == "") break;
    Serial.println(info[i]);
    SerialBT.println(info[i]);
  }
}
