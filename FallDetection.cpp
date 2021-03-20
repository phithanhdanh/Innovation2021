
#include "FallDetection.h"
#include "Arduino.h"
#include <Wire.h>

FallDetection::FallDetection(){
  n = 0;
  ax = ay = az = gx = gy = gz = 0;
  fall = trigger1 = trigger2 = trigger3 = false;
  demSoRung = 0;
  angleChange=0;
}


void FallDetection::init(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void FallDetection::process(){
   n = 0;
   printenable = false;
   
   gx = (GyX+540)/131.07;
   gy = (GyY+200)/131.07;
   gz = (GyZ-287)/131.07;
  
   ax = (abs(AcX)+870)*9.8/16384.00;
   ay = (abs(AcY))*9.8/16384.00;
   az = (abs(AcZ)+964)*9.8/16384.00;
  
   // calculating Amplitute vactor for 3 axis
   float Raw_AM = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
  // int AM = Raw_AM * 10;  // as values are within 0 to 1, I multiplied 
   float AM=Raw_AM;                     // it by for using if else conditions 
  data[n++] = String(AM);
  
   if (trigger3==true){
      trigger3count++;
      
      data[n++] = String(trigger3count);
      if (trigger3count>=10){ 
         angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
         //delay(10);
         data[n++] = String(angleChange); 
  
         if ((angleChange>=0) && (angleChange<=10) ){ //if orientation changes remains between 0-10 degrees
             fall=true; trigger3=false; trigger3count=0;
             data[n++] = String(angleChange);
               }
         else{ //user regained normal orientation
            trigger3=false; trigger3count=0;
            data[n++] = "TRIGGER 3 DEACTIVATED";
            printenable = true;
         }
       }
    }
   if (fall==true){ //in event of a fall detection
     data[n++] = "FALL DETECTED";
     digitalWrite(11, LOW);
     delay(20);
     digitalWrite(11, HIGH);
     fall=false;
     demSoRung=0;
     }
   if (trigger2count>=6){ //allow 0.5s for orientation change
     trigger2=false; trigger2count=0;
     data[n++] = "TRIGGER 2 DECACTIVATED";
     printenable = true;
     }
   if (trigger1count>=6){ //allow 0.5s for AM to break upper threshold
     trigger1=false; trigger1count=0;
     data[n++] = "TRIGGER 1 DECACTIVATED";
     printenable = true;
     }
   if (trigger2==true){
     trigger2count++;
     angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); data[n++] = String(angleChange);
     if (angleChange>=150 && angleChange<=400 ){ //if orientation changes by between 80-100 degrees
       trigger3=true; trigger2=false; trigger2count=0;
       data[n++] = String(angleChange);
       data[n++] = "TRIGGER 3 ACTIVATED";
       printenable = true;
         }
     }
   if (trigger1==true){
     trigger1count++;
     if (AM>=19
     ){ //if AM breaks upper threshold (3g)
       trigger2=true;
       data[n++] = "TRIGGER 2 ACTIVATED";
       printenable = true;
       trigger1=false; trigger1count=0;
       }
     }
   if (AM>=18  && trigger2==false){ //if AM breaks lower threshold (0.4g)
     trigger1=true;
     data[n++] = "TRIGGER 1 ACTIVATED";
     printenable = true;
     }

   data[n++] = "";
  //It appears that delay is needed in order not to clog the port
   delay(100);
}

void FallDetection::read(){   
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
   Wire.endTransmission(false);
   
   Wire.requestFrom(MPU_addr,14,1);  // request a total of 14 registers
   AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
   AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
   AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
   GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
   GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
   GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)   
}
