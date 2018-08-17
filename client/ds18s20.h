#include <OneWire.h>

double getTemp(OneWire* bus){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if (!bus->search(addr)) {
      //no more sensors on chain, reset search
      bus->reset_search();
      return -1000;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  bus->reset();
  bus->select(addr);
  bus->write(0x44,1); // start conversion, with parasite power on at the end

  byte present = bus->reset();
  bus->select(addr);
  bus->write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = bus->read();
  }

  bus->reset_search();

  byte MSB = data[1];
  byte LSB = data[0];
  double tempRead = ((MSB << 8) | LSB); //using two's compliment
  double temperatureSum = tempRead / 16;

  if (temperatureSum > 2000)
    return temperatureSum - 4096;

  return temperatureSum;

}
