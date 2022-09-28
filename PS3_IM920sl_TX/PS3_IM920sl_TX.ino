#include <SoftwareSerial.h>
#include <PS3USB.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS3USB PS3(&Usb);

SoftwareSerial IM920Serial(7,8);//RX,TX
const int BusyPin = 2;
const String str1;
char input[30];
int i ;
unsigned int latestLeftX, latestLeftY, latestRightX, latestRightY, latestL2, latestR2;
char buttonFlag;
//int j = 0;

void setup() {
  IM920Serial.begin(115200);
  Serial.begin(115200);
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
    if (Usb.Init() == -1) {
      Serial.print(F("\r\nOSC did not start"));
      while (1); //halt
    }
  Serial.print(F("\r\nPS3 USB Library Started"));
//  IM920Serial.print("ENWR\r");
//  IM920Serial.print("STCH 37\r");
//  IM920Serial.print("DSWR\r");
  pinMode(BusyPin, OUTPUT);
  digitalWrite(BusyPin, LOW);
  latestLeftX = 120;
  latestLeftY = 120;
  latestRightX = 120;
  latestRightY = 120;
  latestL2 = 0;
  latestR2 = 0;
  buttonFlag = '0';
}

//void loop(){ //send test
//      IM920Serial.print("TXDA FFFFFFFF\r");
//      i = 0;
//      while(IM920Serial.available()){
//        input[i] = IM920Serial.read();
//        Serial.print(input[i++]);
//      }
//      delay(60);
//}


void loop(){
  Usb.Task();
   
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if(latestLeftX != PS3.getAnalogHat(LeftHatX) || latestLeftY != PS3.getAnalogHat(LeftHatY) || latestRightX != PS3.getAnalogHat(RightHatX) || latestRightY != PS3.getAnalogHat(RightHatY)){
      latestLeftX = PS3.getAnalogHat(LeftHatX);
      latestLeftY = PS3.getAnalogHat(LeftHatY);
      latestRightX = PS3.getAnalogHat(RightHatX);
      latestRightY = PS3.getAnalogHat(RightHatY);
//      Serial.print(F("\r\nLeftHatX: "));
//      Serial.print(String(latestLeftX, HEX));
//      Serial.print(F("\tLeftHatY: "));
//      Serial.print(String(latestLeftY, HEX));
//      Serial.print(F("\tRightHatX: "));
//      Serial.print(String(latestRightX, HEX));
//      Serial.print(F("\tRightHatY: "));
//      Serial.print(String(latestRightY, HEX));
//      Serial.print(F("\r\nLeftHatX: "));
//      Serial.print(latestLeftX);
//      Serial.print(F("\tLeftHatY: "));
//      Serial.print(latestLeftY);
//      Serial.print(F("\tRightHatX: "));
//      Serial.print(latestRightX);
//      Serial.print(F("\tRightHatY: "));
//      Serial.print(latestRightY);
      IM920Serial.print("TXDA");
      IM920Serial.print(strPad(String(latestLeftX, HEX), 2));
      IM920Serial.print(strPad(String(latestLeftY, HEX), 2));
      IM920Serial.print(strPad(String(latestRightX, HEX), 2));
      IM920Serial.print(strPad(String(latestRightY, HEX), 2));
      IM920Serial.print("\r\n");
      delay(60);//[ms]
    }
    if (latestL2 != PS3.getAnalogButton(L2) || latestR2 != PS3.getAnalogButton(R2)) {
      latestL2 = PS3.getAnalogButton(L2);
      latestR2 = PS3.getAnalogButton(R2);
      IM920Serial.print("TXDA 0000");
      IM920Serial.print(strPad(String(latestL2, HEX), 2));
      IM920Serial.print(strPad(String(latestR2, HEX), 2));
      IM920Serial.print("\r\n");
      delay(60);
    }

//    if (PS3.getButtonClick(PS))
//      Serial.print(F("\r\nPS"));

    if (PS3.getButtonClick(TRIANGLE))buttonFlag = '1';
    if (PS3.getButtonClick(CIRCLE))buttonFlag = '2';
    if (PS3.getButtonClick(CROSS))buttonFlag = '3';
    if (PS3.getButtonClick(SQUARE))buttonFlag = '4';

    if (PS3.getButtonClick(UP))buttonFlag = '5'; 
    if (PS3.getButtonClick(RIGHT))buttonFlag = '6';
    if (PS3.getButtonClick(DOWN))buttonFlag = '7';
    if (PS3.getButtonClick(LEFT))buttonFlag = '8';

    if (PS3.getButtonClick(L1))buttonFlag = '9';
    if (PS3.getButtonClick(L3)){
      buttonFlag = 'a';
      Serial.print(F("\r\nL3"));
    }
    if (PS3.getButtonClick(R1))buttonFlag = 'b';
    if (PS3.getButtonClick(R3))buttonFlag = 'c';

    if (PS3.getButtonClick(SELECT))buttonFlag = 'd';
    if (PS3.getButtonClick(START))buttonFlag = 'e';
    
    if(buttonFlag != '0'){
       IM920Serial.print("TXDA ffff0");
       IM920Serial.print(buttonFlag);
       IM920Serial.print("00\r\n");
       delay(60);
       IM920Serial.print("TXDAffff0000\r\n");
       buttonFlag = '0';
    }
  }
}

String strPad(String str,int zeroCount){  //IM920slでは、1桁の数値を送れません。この関数を使って0埋めしてください (数値, 桁数)
  String returnStr = "";
  
  if(zeroCount <= str.length()){
    return str;
  }
  for(int i = 0;i < zeroCount - str.length();i++){
    returnStr += '0';
  }
  return returnStr + str;
}
