#ifndef Lampu_h
#define Lampu_h
#include "Arduino.h"

class Lampu {
    private:
        int led1; 
        int led2;
        int led3;
        int led4;
        int led5;
        int led6;

    public:
        Lampu(int lampu1, int lampu2, int lampu3, int lampu4, int lampu5, int lampu6);
        void pinSetup();
        void Sequence();
}; 
#endif