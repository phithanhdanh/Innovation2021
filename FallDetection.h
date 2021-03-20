
#ifndef FALLDETECTION_H
#define FALLDETECTION_H
#include "Arduino.h"

class FallDetection {
  
    #define MPU_ADDRESS 0x68
  
  public:

    FallDetection();
    void init();
    void process();
    String *data;


  private:
  
    void read();
    
    const int MPU_addr = MPU_ADDRESS;  // I2C address of the MPU-6050
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    float ax, ay, az, gx, gy, gz;
  
    int demSoRung; //đếm số lần cảm biến xuất hiện giá trị 1 
  
    boolean fall; //stores if a fall has occurred
    boolean trigger1; //stores if first trigger (lower threshold) has occurred
    boolean trigger2; //stores if second trigger (upper threshold) has occurred
    boolean trigger3; //stores if third trigger (orientation change) has occurred
  
    byte trigger1count; //stores the counts past since trigger 1 was set true
    byte trigger2count; //stores the counts past since trigger 2 was set true
    byte trigger3count; //stores the counts past since trigger 3 was set true
    int angleChange;

    int n;

};


#endif
