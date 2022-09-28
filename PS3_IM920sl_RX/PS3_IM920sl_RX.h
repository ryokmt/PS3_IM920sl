#ifndef PS3_IM920sl_H
#define PS3_IM920sl_H

#include "mbed.h"
#include "BufferedSerial.h"

class PS3_IM920sl : public BufferedSerial{
    private:
    
    volatile int j, i;
    typedef uint8_t byte;
    volatile unsigned int _decD[8];
    volatile int controller[4];
    volatile float rlXY[4];
    volatile float rlZ[2];
    volatile int button;

    // Serial _serial;

    // void _serial_RX();

    void fixData(byte *data);

    public:
  
    volatile float lx, ly, rx, ry, z, lr, lth, rr, rth;
    // volatile bool down_up, down_left, down_down, down_right, down_triangle, down_square, down_cross, down_circle, down_l1, down_r1, down_select, down_start;
    volatile bool raise_up, raise_left, raise_down, raise_right, raise_triangle, raise_square, raise_cross, raise_circle, raise_l1, raise_r1, raise_select, raise_start;

    PS3_IM920sl(PinName TX, PinName RX);
    byte readMsg (byte * data, const byte length, byte condition, unsigned long timeout);

    void task();

    void ps3_RX();    
    
};

#endif


