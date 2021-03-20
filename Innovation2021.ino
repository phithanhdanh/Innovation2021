#define MAX 50
#define DELAY 500

#include <BluetoothSerial.h>
#include <Wire.h>
#include "FallDetection.h"
#include "HeartRate.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
FallDetection mpu;

unsigned long previousmillis;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  Wire.begin();
  mpu.init();

  pinMode(25,INPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  
  previousmillis = millis();
}

void loop() {

  mpu.process();
  if ((millis() - previousmillis > DELAY) && (mpu.printenable)) {
    printdata(mpu.data);
    previousmillis = millis();
  }
}

void printdata(String*info){
  for (int i=0; i < MAX, info[i] != ""; i++){
    Serial.println(info[i]);
    SerialBT.println(info[i]);
    info[i] = "";
  }
}
