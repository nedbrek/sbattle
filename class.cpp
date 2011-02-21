// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// class members

#include "sim.h"
#include "range.h"

// results:
// success 0
// roll    1 (implies oppose)
#define RESULTS_SCS 1

// results possibilities
// add 1                   0
// subtract 1              2
// oppose, p1 rush p2 fall 4
// oppose, p2 rush p1 fall 6
// use 0, 2, 4, 5, 7
static int results[16] = {
// result   P1   P2
   0+2, // ADVN ADVN -> automatic success (0) + subtract 1 (2)
	1+4, // ADVN MNTN -> roll  for success (1) + oppose p1 rush (4)
	0+2, // ADVN HOLD -> automatic success (0) + subtract 1 (2)
	1+4, // ADVN RTRT -> roll  for success (1) + oppose p1 rush (4)
   1+6, // MNTN ADVN -> roll  for success (1) + oppose p2 rush (6)
	0+4, // MNTN MNTN -> automatic success (0) + oppose p1 rush (4)
	0+4, // MNTN HOLD -> automatic success (0) + oppose p1 rush (4)
	1+4, // MNTN RTRT -> roll  for success (1) + oppose p1 rush (4)
   0+2, // HOLD ADVN -> automatic success (0) + subtract 1 (2)
	0+4, // HOLD MNTN -> automatic success (0) + oppose p1 rush (4)
	0+4, // HOLD HOLD -> automatic success (0) + oppose p1 rush (4)
	0+0, // HOLD RTRT -> automatic success (0) + add 1 (0)
   1+6, // RTRT ADVN -> roll  for success (1) + oppose p2 rush (6)
	1+6, // RTRT MNTN -> roll  for success (1) + oppose p2 rush (6)
	0+0, // RTRT HOLD -> automatic success (0) + add 1 (0)
	0+0  // RTRT RTRT -> automatic success (0) + add 1 (0)
};

#define RESULTS_ADD 2
#define RESULTS_OP1 4

//--------------------- range, the distance between the fighters
void Range::update(int p1, int p2, Ship  &s1, Ship  &s2)
{
//const int oldr = cur_;

   const int moves = s1.move() + s2.move();
//if(moves == 0) printf("What?????!\n");
//fflush(stdout);

   --p1;
   --p2;
   int result = results[((p1 & 3) << 2) | (p2 & 3)];

	int success = ~(result & RESULTS_SCS); // automatic success
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

   int upd = 0;
   if(success && (result & RESULTS_ADD)) upd = -1;
   if(success && !(result & RESULTS_ADD)) upd = 1;

   if(success == 2) upd = 0;

   cur_ = Range_Q(int(cur_) + upd);
   if(cur_ > RANGE_EXTR) cur_ = RANGE_EXTR;
   if(cur_ < RANGE_PTBK) cur_ = RANGE_PTBK;
//if(oldr != cur_) printf("Moved:%d\t", cur_);

	mod_ = MODIFIER[cur_];

#ifdef DEBUG
if(oldr != cur_) printf("Moved:%d\n", cur_);
#endif
}

//--------------------- class hit, a stack of damage packets
// read, return top of stack
int Hit::read(void)
{
   return(damage[number-1]);
}

// remove, pop off stack
void Hit::remove(void)
{
   --number;
   damage[number]=0xdead;
}

// add, push onto stack
void Hit::add(int d)
{
   damage[number] = d;
   ++number;
}

/*Hit::Hit(const Hit &h)
{
//   if(damage) delete[] damage;

   number = h.number;
   damage = new(int[number]);

   memcpy(damage, h.damage, number*sizeof(int));
}*/

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

