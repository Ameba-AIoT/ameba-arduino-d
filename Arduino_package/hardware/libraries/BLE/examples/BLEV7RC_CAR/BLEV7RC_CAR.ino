/*******************************************************
 * BLEV7RC_CAR.ino - BLE Remote Control Car Arduino Code
 * 
 * Author: Kevin Chen
 * Date: 2023/7/27
 * 
 * Version: 1.0.0
 * 
 * This code was written by Kevin Chen.
 * 
 * This is an open-source program, and it can be freely used, modified, and distributed under the following conditions:
 * 
 * 1. The original copyright notice must not be removed or modified.
 * 2. Projects or products using this code must acknowledge the original author's name and the source in applicable documentation, websites, or other related materials.
 * 3. Any derivative work based on this code must state its origin and retain the original copyright notice in its documentation.
 * 4. This code must not be used for any activities that may infringe upon the rights of others, be unlawful, or harmful, whether in a commercial or non-commercial environment.
 * 
 * This code is provided "as is" with no warranty, expressed or implied. The author is not liable for any losses or damages caused by using the code.
 * 
 * Users of BLEV7RC_CAR.ino assume all risks associated with its use, and the author shall not be held responsible for any consequences.
 * 
 * For more information about our company, please visit: www.makdev.net
 * 
 *******************************************************/



#include "BLEDevice.h"

#define UART_SERVICE_UUID      "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define STRING_BUF_SIZE 100
#define MaxNumValue      2

#define value1  0
#define value2  1


#define MotoA_1A 6
#define MotoA_1B 12
#define MotoB_1A 2
#define MotoB_1B 3

typedef struct{
    bool reciveCMDFlag;
    int  ReciveValue;

}_rCMD;

BLEService UartService(UART_SERVICE_UUID);
BLECharacteristic Rx(CHARACTERISTIC_UUID_RX);
BLECharacteristic Tx(CHARACTERISTIC_UUID_TX);
BLEAdvertData advdata;
BLEAdvertData scndata;
bool notify = false;
uint8_t Count;

_rCMD bleReciveData[MaxNumValue];

void backward(){
  digitalWrite(MotoA_1A,HIGH);
  analogWrite(MotoA_1B,5);  

  digitalWrite(MotoB_1A,HIGH);
  analogWrite(MotoB_1B,5);

  delay(50);
}


void forward(){
  digitalWrite(MotoA_1A,LOW);
  analogWrite(MotoA_1B,250);

  digitalWrite(MotoB_1A,LOW);
  analogWrite(MotoB_1B,250);
  
  delay(50);
}

void turnRight(){

  digitalWrite(MotoA_1A,HIGH);
  analogWrite(MotoA_1B,5);

  digitalWrite(MotoB_1A,LOW);
  analogWrite(MotoB_1B,250);

  delay(50);
}

void turnLeft(){

  digitalWrite(MotoA_1A,LOW);
  analogWrite(MotoA_1B,250);

  digitalWrite(MotoB_1A,HIGH);
  analogWrite(MotoB_1B,5);

  delay(50);
}

void BrakeAll(){
  digitalWrite(MotoA_1A,LOW);
  analogWrite(MotoA_1B,0);

  digitalWrite(MotoB_1A,LOW);
  analogWrite(MotoB_1B,0);
  
  delay(50);
}

void readCB (BLECharacteristic* chr, uint8_t connID) {
    //printf("Characteristic %s read by connection %d \n", chr->getUUID().str(), connID);
}

void writeCB (BLECharacteristic* chr, uint8_t connID) {
    //printf("Characteristic %s write by connection %d :\n", chr->getUUID().str(), connID);
    if (chr->getDataLen() > 0) {
      ParseCMDString(chr->readString());

        
        Serial.print("Received string: ");
        Serial.print(chr->readString());
        Serial.println();   
        
    }
}

void notifCB(BLECharacteristic* chr, uint8_t connID, uint16_t cccd) {
  if (cccd & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
    //printf("Notifications enabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
    Serial.print("Notifications enabled on Characteristic");
    notify = true;
  } else {
    //printf("Notifications disabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
    Serial.print("Notifications disabled on Characteristic");
    notify = false;
  }
  Serial.print(chr->getUUID().str());
  Serial.print(" for connection");
  Serial.println(connID);
}

void ParseCMDString(String cmd)
{
    int comdLength = cmd.length();
    
    if(cmd.indexOf("SRT") > -1 ){

        int x = 3;
        int ValueIndex = 0;

        while(x < comdLength - 1){
            if(x + 3 < comdLength){
                String _NumString = cmd.substring(x,x + 4);
                //Serial.println(_NumString);

                if(ValueIndex < MaxNumValue){
                    if(bleReciveData[ValueIndex].ReciveValue != _NumString.toInt()){
                        bleReciveData[ValueIndex].ReciveValue = _NumString.toInt();
                        bleReciveData[ValueIndex].reciveCMDFlag = true;
                    }
                }

            }

            ValueIndex++;

            x += 4;
            //Serial.println();
        }
        
    }
}



void setup()
{
	  Serial.begin(115200);

    pinMode(MotoA_1A,OUTPUT);
    pinMode(MotoA_1B,OUTPUT);
    pinMode(MotoB_1A,OUTPUT);
    pinMode(MotoB_1B,OUTPUT);

    digitalWrite(MotoA_1A,LOW);
    digitalWrite(MotoA_1B,LOW);
    digitalWrite(MotoB_1A,LOW);
    digitalWrite(MotoB_1B,LOW);

    advdata.addFlags(GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);
    advdata.addCompleteName("AMEBA_BLE_DEV");
    scndata.addCompleteServices(BLEUUID(UART_SERVICE_UUID));

    Rx.setWriteNRProperty(true);
    Rx.setWritePermissions(GATT_PERM_WRITE);
    Rx.setWriteCallback(writeCB);
    Rx.setBufferLen(STRING_BUF_SIZE);

    Tx.setReadProperty(true);
    Tx.setReadPermissions(GATT_PERM_READ);
    Tx.setReadCallback(readCB);
    Tx.setNotifyProperty(true);
    Tx.setCCCDCallback(notifCB);
    Tx.setBufferLen(STRING_BUF_SIZE);
    
    UartService.addCharacteristic(Rx);
    UartService.addCharacteristic(Tx);

    BLE.init();
    BLE.configAdvert()->setAdvData(advdata);
    BLE.configAdvert()->setScanRspData(scndata);
    BLE.configServer(1);
    BLE.addService(UartService);

    BLE.beginPeripheral();
   
}

void loop()
{
    while(Count < MaxNumValue) {
        if(bleReciveData[Count].reciveCMDFlag){
            bleReciveData[Count].reciveCMDFlag = false;

            if(abs( bleReciveData[value1].ReciveValue- 1500) < 100 && abs(bleReciveData[value2].ReciveValue - 1500) < 100){
                BrakeAll();
            }else if(abs(bleReciveData[value1].ReciveValue - 1500) > abs(bleReciveData[value2].ReciveValue - 1500) ){
                if(bleReciveData[value1].ReciveValue > 1500){
                    turnRight();
                }
                else{
                    turnLeft();
                }
            }else{
                if(bleReciveData[value2].ReciveValue > 1500){
                    forward();
                }
                else{
                    backward();
                }
            }
        }
        Count++;
    }
    Count = 0;



    delay(1);	
}
