#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <iostream>
#include "bjorklund.h"

#define SEQUENCE_ALGORITHM_ARRAY_SIZE 10

template <typename T>
class Sequence
{
public:
  Sequence()
    : length(1)
    , pos(0)
  {
  }

  void calculate(uint8_t steps, uint8_t fills)
  {
    Bjorklund<T, SEQUENCE_ALGORITHM_ARRAY_SIZE> algo;
    T newbits;
    newbits = algo.compute(steps, fills);
    length = steps;
    bits = newbits;
  }

  void print()
  {
    for (int i = 0; i < length; ++i)
    {
      std::cout << (next() ? 'x' : '-');
    }
    std::cout << std::endl;
  }

  void reset()
  {
    pos = offset % length;
  }

  void rotate(int8_t steps)
  {
    pos = (length + pos + steps - offset) % length;
    offset = steps;
  }

  bool next()
  {
    if (pos >= length)
      pos = 0;
    return bits & (1UL << pos++);
  }
  
  uint8_t getLength() const{ return length; }
  uint8_t getPos() const{ return pos; }
  T getBits() const {return bits; }
private:
  T bits;
  uint8_t length;
  int8_t offset;
  volatile uint8_t pos;
};

#endif /* _SEQUENCE_H_ */