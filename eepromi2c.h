#include <Arduino.h>
#include <Wire.h>
#include <AT24CX.h>

AT24CX mem;


template<class T> int eeWrite(int ee, const T& value) {
  uint8_t* p = (uint8_t*)&value;
  mem.write((unsigned)ee, p, (int) sizeof(value));
  return (int) sizeof(value);
}

template<class T> int eeRead(int ee, T& value) {
  uint8_t* p = (uint8_t*)&value;
  mem.read((unsigned)ee, p, (int) sizeof(value));
  return (int) sizeof(value);;
}
