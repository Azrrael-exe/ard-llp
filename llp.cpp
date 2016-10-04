#include "llp.h"

DataPack::DataPack(){
  cursor = 0;
}

bool DataPack::addData(uint8_t index, byte msb, byte lsb){
  if(cursor<87){
    buffer[cursor] = index; cursor++;
    buffer[cursor] = msb; cursor++;
    buffer[cursor] = lsb; cursor++;
    return true;
  }
  return false;
}

bool DataPack::addData(uint8_t index, int16_t data){
  byte data_msb = byte(data >> 8);
  byte data_lsb = byte(data);
  return addData(index, data_msb, data_lsb);
}

void DataPack::clear(){
  for(int i=0;i<90;i++){
    buffer[i] = 0;
  }
  cursor = 0;
}

int8_t DataPack::calCheckSum(){
  byte sum = 0;
  for(int i=0;i<cursor; i++){
    sum+=buffer[i];
  }
  return byte(0xFF-sum);
}

void DataPack::write(Stream& inp){
  inp.write(0x7E);
  inp.write(cursor);
  for(int i=0;i<cursor;i++){
    inp.write(buffer[i]);
  }
  inp.write(calCheckSum());
  clear();
}

bool DataPack::available(Stream& inp){
  if(inp.available()){
    uint8_t head[2];
    inp.readBytes(head,2);
    if(head[0] == 0x7E){
      uint8_t len = head[1];
      inp.readBytes(inp_buffer, len+1);
      uint8_t rec_checksum = inp_buffer[len];
      uint16_t sum = 0;
      for(int i=0;i<len; i++){
        sum += inp_buffer[i];
      }
      sum = 0xFF - byte(sum);
      if(rec_checksum == sum){
        inp_data = len/3;
        return true;
      }
      else{
        Serial.println("Invalid Check Sum");
        Serial.print("Rec: ");Serial.println(rec_checksum);
        Serial.print("Cal: ");Serial.println(sum);
      }
    }
  }
  return 0;
}

uint8_t* DataPack::getIndexs(){
  uint8_t* out = (uint8_t *) calloc(inp_data, sizeof(uint8_t));
  for(int i=0;i<inp_data;i++){
    out[i] = inp_buffer[(3*i)];
  }
  return out;
}

uint8_t DataPack::inWaiting(){
  return inp_data;
}

uint16_t DataPack::getData(uint8_t key){
  for(int i = 0; i < inp_data*3 ; i+=3){
    if(key == inp_buffer[i]){
      return (inp_buffer[i+1] << 8) | (inp_buffer[i+2]);
    }
  }
  return 0;
}
