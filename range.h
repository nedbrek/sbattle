#ifndef RANGE_H
#define RANGE_H

struct Ship;

//----------------------------------------------------------------------------
// Range values
enum Range_Q
{
 RANGE_PTBK,
 RANGE_SHRT,
 RANGE_MEDM,
 RANGE_LONG,
 RANGE_EXTR,
 RANGE_MAX
};

const int MODIFIER[RANGE_MAX] = {30, 15, 0, -20, -40};

// effective range between two ships
class Range
{
protected:
   Range_Q cur_; // quantized range
   int     mod_;

   //Range(void); // not implemented

public:
   // start at extreme
    Range(void): cur_(RANGE_EXTR)
    {
       mod_= MODIFIER[cur_];
    }

   // move according to plans
   void update(int p1, int p2, Ship &s1, Ship &s2);

	int mod(void) const { return mod_; }
	Range_Q cur(void) const { return cur_; }
};

#endif

