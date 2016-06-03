// from https://github.com/pingdynasty/EuclideanSequencer

#ifndef _BJORKLUND_H_
#define _BJORKLUND_H_

/**
   Implementation of the Bjorklund algorithm.
   Algorithm based on SNS-NOTE-CNTRL-99
   The Theory of Rep-Rate Pattern Generation in the SNS Timing System
   E. Bjorklund
*/

/**
  the size of the count and remainder arrays is given by:
  l+1=logτ( m * sqrt(5) )−3
  where τ = (1 + sqrt(5) ) / 2 
  (the Golden Ratio)
  by logarithmic base change rule logb(x) = logc(x) / logc(b) 
  we have
  l+1 = ln( m * sqrt(5) ) / ln( (1 + sqrt(5) ) / 2 )
  for m=128, l+1 < 12
  for m=32, l+1 < 9
*/

template<typename T, uint8_t BJORKLUND_ARRAY_SIZE>
class Bjorklund {
public:
  T compute(int8_t slots, int8_t pulses){
    bits = 0UL;
    pos = 0;
    if(!pulses)
      return bits;
    /* Figure 11 */
    int8_t divisor = slots - pulses;
    remainder[0] = pulses; 
    int8_t level = 0; 
    int8_t cycleLength = 1; 
    int8_t remLength = 1;
    do { 
      count[level] = divisor / remainder[level]; 
      remainder[level+1] = divisor % remainder[level]; 
      divisor = remainder[level]; 
      int8_t newLength = (cycleLength * count[level]) + remLength; 
      remLength = cycleLength; 
      cycleLength = newLength; 
      level = level + 1;
    }while(remainder[level] > 1);
    count[level] = divisor; 
    if(remainder[level] > 0)
      cycleLength = (cycleLength * count[level]) + remLength;
    build(level);
    return bits;
  }

private:
  T bits;
  uint8_t pos;
  int8_t remainder[BJORKLUND_ARRAY_SIZE];
  int8_t count[BJORKLUND_ARRAY_SIZE];

  void build(int8_t level){
    if(level == -1){
      //     pos++;
      bits &= ~(1UL<<pos++);
    }else if(level == -2){
      bits |= 1UL<<pos++;
    }else{ 
      for(int8_t i=0; i < count[level]; i++)
	build(level-1); 
      if(remainder[level] != 0)
	build(level-2); 
    }
  }
};

#endif /* _BJORKLUND_H_ */