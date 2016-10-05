#ifndef frame_h
#define fame_h

#include <Arduino.h>

class DataPack{
public:
  DataPack();
  bool addData(uint8_t index, byte msb, byte lsb);
  bool addData(uint8_t index, int16_t data);
  void clear();
  void write(Stream& inp);
  bool available(Stream& inp);
  uint16_t getData(uint8_t key);
  uint8_t inWaiting();
  uint8_t* getKeys();
  bool hasKey(uint8_t);
private:
  int8_t calCheckSum();
  uint8_t buffer[90];
  uint8_t cursor;
  uint8_t inp_buffer[90];
  uint8_t inp_data;
};

#endif
