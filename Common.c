#include "xinaga.h"

byte ReadBit(byte byteToRead, char bit)//These are old
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
void WriteBit(byte *byteToSet, char bit, bool value)//These are old
{
  if (value)
  {
    bit = 1 << bit;
  }
  else
  {
    bit = 0 << bit;
  }
  *byteToSet = *byteToSet | bit;
}

bool CheckBit(byte source, byte position) //These are old
{
  return (source & (1<<position));
}

byte SetBit(byte byteToSet , byte k) //These work right
{
  byteToSet = byteToSet | (1 << (k - 1));
  return byteToSet;
}
byte ClearBit(byte byteToSet , byte k) //These work right
{
  byteToSet = byteToSet & (~(1 << (k)));
  return byteToSet;
}