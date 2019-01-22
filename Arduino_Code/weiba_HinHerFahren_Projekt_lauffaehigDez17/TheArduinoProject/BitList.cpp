#include "BitList.h"

BitList::BitList(unsigned int byteCount)
{
  dieListe = new unsigned char[byteCount];
  for (unsigned int i = 0; i < byteCount; i++)
  {
    dieListe[i] = 0;
  }
}

BitList::~BitList()
{
  delete[] dieListe;
  dieListe = 0;
}

bool BitList::setBit(unsigned int index, unsigned char value_0_1)
{
  unsigned int bitIndex  = index % 8;
  unsigned int byteIndex = index - bitIndex;

  if (byteIndex >= byteCount) return false;
  
  if (value_0_1 == 1)
  {
    dieListe[byteIndex] |= (1 << bitIndex);
  }
  else
  {
    dieListe[byteIndex] &= (0xFF - (1 << bitIndex));
  }
  return true;
}

unsigned char BitList::getBit(unsigned int index, bool* success)
{
  unsigned int bitIndex  = index % 8;
  unsigned int byteIndex = index - bitIndex;
  if (success != 0)
  {
    *success = (byteIndex < byteCount);
  }
  
  return (dieListe[byteIndex] & (1 << bitIndex)) >> bitIndex;
}
