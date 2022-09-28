#include "PS3_IM920sl.h"

typedef unsigned int word;
typedef uint8_t byte;
typedef uint8_t boolean;
typedef void (*voidFuncPtr)(void);
Timer lap1;

// Serial pc(USBTX, USBRX, 921600); //Serial Tx pin name, Serial Rx pin name, Frequency
// void pc_RX();

PS3_IM920sl::PS3_IM920sl( PinName rx, PinName tx):BufferedSerial(rx, tx){
  return;
}

byte PS3_IM920sl::readMsg(byte *data, const byte length, byte condition, unsigned long timeout){
  unsigned long start_time = lap1.read_ms();
  bool flag = 0;
  byte tmpData = 0;
  int num = 2;

  // tmpData = getc();
  // while(readable() > 0){
  //   pc.printf()
  //   // pc.printf("read!");
  //   // if(tmpData == 'f')pc.printf("%d,",tmpData);
  //   clear_buf();
  // }

  while((lap1.read_ms() - start_time) < timeout){
    if (readable() > 0){
      tmpData = getc();
      if((tmpData == condition) && (!flag)){
        // pc.printf("%c",tmpData);
        data[0] = tmpData;
        flag = 1;
        num = 1;
        start_time = lap1.read_ms();
      }else{
        if(flag){
          if(num < length){
            if(tmpData == '\n' && tmpData == ':') return 1; //bad data
            // pc.printf("%c",tmpData);
            data[num] = tmpData;
            num++;
          }
          if(num >= length){
            clear_buf();
            return 1;  // overflow
          }
        }
      }
    }
  }
  
  clear_buf();
  return 0;  // timeout
} // end of recvMsg

void PS3_IM920sl::task(){
  byte data[11];
  lap1.start();

  readMsg(data, 12, ':', 1000);;
  fixData(data);
  
  lx = rlXY[0];
  ly = rlXY[1];
  lr = sqrt( (lx)*(lx) + (ly)*(ly) );
  lth = atan2( rlXY[1] * 120.0, rlXY[0] * 120.0 ); //Unimplemented
  rx = rlXY[2];
  ry = rlXY[3];
  rr = sqrt( (rx)*(rx) + (ry)*(ry) );
  rth = atan2( rlXY[3] * 120.0, rlXY[2] * 120.0 );
  z = rlZ[0] + rlZ[1];
  // pc.printf("lx=%lf, ly=%lf, rx=%lf, ry=%lf\r\n",rlXY[0],rlXY[1],rlXY[2],rlXY[3]);
}

void PS3_IM920sl::fixData(byte *data){
  if(data[0] == ':'){
    j = 0;
    for(i = 1; i < 12; i++){
    //  pc.printf("%c\r\n",data[i]);
      if(data[i] != ',') {
        if(data[i] > 64){
          _decD[j++] = data[i] - '7';
        }else{
          _decD[j++] = data[i] - '0';
        }
      }
    }

    if(_decD[0] == _decD[1] && _decD[2] == _decD[3] && _decD[0] == _decD[2] && _decD[0] == 15 ){
      button = _decD[4] *16 + _decD[5];
      switch(button){//以下の変数は、trueになった60ms後にすぐfalseに戻るため注意。
        case 0x00:
          raise_triangle = false;
          raise_circle = false;
          raise_cross = false;
          raise_square = false;
          raise_up = false;
          raise_right = false;
          raise_down = false;
          raise_left = false;
          raise_l1 = false;
          raise_r1 = false;
          raise_select = false;
          raise_start = false;
          break;
        case 0x01:
          raise_triangle = true;
          break;
        case 0x02:
          raise_circle = true;
          break;
        case 0x03:
          raise_cross = true;
          break;
        case 0x04:
          raise_square = true;
          break;
        case 0x05:
          raise_up = true;
          break;
        case 0x06:
          raise_right = true;
          break;
        case 0x07:
          raise_down= true;
          break;
        case 0x08:
          raise_left = true;
          break;
        case 0x09:
          raise_l1 = true;
          break;
        case 0x0a:
          //L3  = true;
          break;
        case 0x0b:
          raise_r1 = true;
          break;
        case 0x0c:
          //R3  = true;
          break;
        case 0x0d:
          raise_select = true;
          break;
        case 0x0e:
          raise_start = true;
          break;
        default:
          break;
      }

    }else if(_decD[0] == _decD[1] && _decD[2] == _decD[3] && _decD[0] == _decD[2] && _decD[0] == 0 ){
      j = 0;
      for(i = 4; i < 8; i++){
        controller[j++] =(_decD[i] * 16 + _decD[++i]); 
      }
      rlZ[0] = (float)controller[0] / 255;
      rlZ[1] = -(float)controller[1] / 255;
    }else{
      j = 0;
      for(i = 0; i < 8; i++){
        controller[j++] =(_decD[i] * 16 + _decD[++i])-128; 
      }

      for(i = 0; i < 4; i++){
        if(controller[i] <= 0){
          rlXY[i] = (float)controller[i] /128.0;
        }else{
          rlXY[i] = (float)controller[i] /127.0;
        }
      }
    }

  }
}
