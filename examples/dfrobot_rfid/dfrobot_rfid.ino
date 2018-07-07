#include "RfidReader.h"

RfidReader rfid(Serial1);

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop(){
  
  uint8_t tag[12];
  if(rfid.readTagId(tag)){
    Serial.print("Successfully read tag: ");
    for(int i = 0; i < 12; i++){
      Serial.print(tag[i], HEX);
      Serial.print(" "); 
    }
    Serial.println();
  }
  
}
