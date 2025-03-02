
// EEPROM update function for esp32
void EEPROMupdate(int p_address, byte value) {
  byte oldVal = EEPROM.read(p_address);
  if(oldVal != value)
      EEPROM.write(p_address, value);
}

void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);
  EEPROMupdate(p_address, lowByte);
  EEPROMupdate(p_address + 1, highByte);
  delay(10);
}
//This function will read a 2-byte integer from the EEPROM at the specified address and address + 1
int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

inline int8_t eeprom(int address, byte x = 0, byte y = 0, byte yDim = 1) {
  return EEPROM.read(address + x * yDim + y);
}

void flushEEPROM(char b = -1) {
  for (int j = 0; j < 1024; j++) {
    EEPROMupdate(j, b);
  }
}

void printEEPROM() {
  for (int i = 0; i < 1024; i++) {
    PT(eeprom(i));
    PT('\t');
    if ((i % 16) == 15){
      PTL();
      delay(10);
    }
  }
}

template <typename T> void printEEPROMList(int EEaddress, byte len = DOF) {
  for (byte i = 0; i < len; i++) {
    PT((T)(EEPROM.read(EEaddress + i)));
    PT('\t');
  }
  PTL();
}

void saveCalib(int8_t *var) {
  for (byte i = 0; i < DOF; i++) {
    EEPROMupdate(CALIB + i, var[i]);
    //    calibratedZeroPosition[i] = EEPROMReadInt(ZERO_POSITIONS + i * 2) + float(var[i]) * eeprom(ROTATION_DIRECTION, i);
    EEPROMWriteInt(CALIBRATED_ZERO_POSITIONS + i * 2, EEPROMReadInt(ZERO_POSITIONS + i * 2) + float(var[i]) * eeprom(ROTATION_DIRECTION, i));
  }
}

void saveMelody(int &address, byte melody[], int len ) {
  EEPROMupdate(address--, len);
  for (byte i = 0; i < len; i++)
    EEPROMupdate(address --, melody[i]);
  PTL(address);
}

inline int loadAngleLimit(byte idx, byte dim) {
  return EEPROMReadInt(ANGLE_LIMIT + idx * 4 + dim * 2);
}
