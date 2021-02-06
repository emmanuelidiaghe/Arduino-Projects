 #include <EEPROM.h>
#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
	  EEPROM.write(ee++, *p++);
    return i;
}

template <class U> int EEPROM_readAnything(int ff, U& value1)
{
    byte* q = (byte*)(void*)&value1;
    unsigned int j;
    for (j = 0; j < sizeof(value1); j++)
	  *q++ = EEPROM.read(ff++);
    return j;
}