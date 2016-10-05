#include "llp.h"

DataPack input = DataPack();

// Example frame [7E 06 A1 45 78 A2 01 AB 53]

void setup(){
  Serial.begin(9600);
}

void loop(){
  if(input.available(Serial)){
    uint8_t counter = input.inWaiting();
    uint8_t* data_keys = input.getKeys();
    for(int i=0;i<counter;i++){
      Serial.print("Key: ");Serial.println(data_keys[i]);
      Serial.print("Data: ");Serial.println(input.getData(data_keys[i]));
    }
    delete(data_keys);
    if(input.hasKey(0xA1)){
      Serial.println("Key Found!");
    }
  }
}
