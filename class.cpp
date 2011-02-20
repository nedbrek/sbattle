// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// class members

#include "sim.h"
#include "range.h"

// Imperial Destroyer stats
#define IMP_N "Imperial Destroyer"
#define IMP_W_NUM 4
#define IMP_S_MAX 10
#define IMP_H_MAX 10
#define IMP_MOVE  2

// Biopod Ship stats
#define BUG_N "Biopod Ship"
//#define BUG_W_NUM 4
#define BUG_W_NUM 3
#define BUG_S_MAX 5
#define BUG_H_MAX 20
#define BUG_MOVE  3

// results possibilities:
// ADVN ADVN 0
// ADVN MNTN 1
// ADVN HOLD 2
// ADVN RTRT 3
// MNTN ADVN 4
// MNTN MNTN 5
// MNTN HOLD 6
// MNTN RTRT 7
// HOLD ADVN 8
// HOLD MNTN 9
// HOLD HOLD 10
// HOLD RTRT 11
// RTRT ADVN 12
// RTRT MNTN 13
// RTRT HOLD 14
// RTRT RTRT 15

// results:
// success 0
// roll    1 (implies oppose)
#define RESULTS_SCS 1
#define RESULTS_RLL 1
#define RESULTS_ADD 2
#define RESULTS_SUB 2
#define RESULTS_OP1 4
#define RESULTS_OP2 4

// add 1                   0
// subtract 1              2
// oppose, p1 rush p2 fall 4
// oppose, p2 rush p1 fall 6
// use 0, 2, 4, 5
int results[16] = {
          0+2, 1+4, 0+2, 1+4,
          1+6, 0+4, 0+4, 1+4,
          0+2, 0+4, 0+4, 0+0,
          1+6, 1+6, 0+0, 0+0
                 };


//--------------------- class hit, a stack of damage packets
// read, return top of stack
int Hit::read(void)
{
   return(damage[number-1]);
}

// remove, pop off stack
void Hit::remove(void)
{
   number--;
   damage[number]=0xdead;
}

// add, push onto stack
void Hit::add(int d)
{
   damage[number] = d;
   number++;
}

/*Hit::Hit(const Hit &h)
{
//   if(damage) delete[] damage;

   number = h.number;
   damage = new(int[number]);

   memcpy(damage, h.damage, number*sizeof(int));
}*/

//--------------------- range, the distance between the fighters
void Range::update(int p1, int p2, Ship  &s1, Ship  &s2)
{
   int moves = s1.move() + s2.move();

//int oldr = cur_;
//if(moves == 0) printf("What?????!\n");
//fflush(stdout);
   int result, success, upd = 0;
// RESULTS_SCS 1
// RESULTS_RLL 1
// RESULTS_ADD 2 zero
// RESULTS_SUB 2 one
// RESULTS_OP1 4
// RESULTS_OP2 4

   p1--;
   p2--;
   result = results[((p1 & 3) << 2) | (p2 & 3)];
   success = ~(result & RESULTS_SCS); // automatic success
   if(result & RESULTS_OP1)
   {
      success = 2; // check for double failure
      success |= rand() % 100 <  ((s1.move() / (double)moves) * 100);
      success &= rand() % 100 >= ((s2.move() / (double)moves) * 100);
   }
   if(result & !RESULTS_OP1)
   {
      success = 2; // check for double failure
      success |= rand() % 100 <  ((s2.move() / (double)moves) * 100);
      success &= rand() % 100 >= ((s1.move() / (double)moves) * 100);
   }

   if(success && (result & RESULTS_ADD)) upd = -1;
   if(success && !(result & RESULTS_ADD)) upd = 1;

   if(success == 2) upd = 0;

   cur_ = Range_Q(int(cur_) + upd);
   if(cur_ > RANGE_EXTR) cur_ = RANGE_EXTR;
   if(cur_ < RANGE_PTBK) cur_ = RANGE_PTBK;
//if(oldr != cur_) printf("Moved:%d\t", cur_);
   switch(cur_)
   {
      case RANGE_PTBK:
         mod_ = 30;
         break;
      case RANGE_SHRT:
         mod_ = 15;
         break;
      case RANGE_MEDM:
         mod_ = 0;
         break;
      case RANGE_LONG:
         mod_ = -20;
         break;
      case RANGE_EXTR:
         mod_ = -40;
         break;

		default:
			assert(false);
   }
#ifdef DEBUG
if(oldr != cur_) printf("Moved:%d\n", cur_);
#endif
}

void Battle::reset(void)
{
}

///@return 0 Imps/1 Bugs/else draw
int Battle::fight(void)
{
	Ship imp('I');
	Ship bug('B');
	return bug.fight(imp);
}

