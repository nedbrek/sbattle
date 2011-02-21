// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// class members

#include "sim.h"
#include "range.h"

namespace Results {
enum Val
{
	CLOSE            = 0,
	CLOSE_ROLL_P1    = 1, // close if P1 better than P2
	CLOSE_ROLL_P2    = 2, // close if P2 better than P1
	NO_CHANGE        = 4,
	WITHDRAW_ROLL_P1 = 5, // withdraw if P1 better than P2
	WITHDRAW_ROLL_P2 = 6, // withdraw if P2 better than P1
	WITHDRAW         = 8,
	MAX
};
	bool needRoll(Val v)
	{
		return (v & 3) != 0;
	}

	int delta(Val v)
	{
		assert( !needRoll(v) );

		return (v >> 2) - 1;
	}

	Val resolveRoll(Val v, bool p1Success)
	{
		if( !needRoll(v) )
			return v; // no roll need

		if( (v & 1) )
		{
			if( p1Success )
				return Val(int(v) - 1);
		}
		else
		{
			assert( v & 2 );
			if( !p1Success )
				return Val(int(v) - 2);
		}

		return NO_CHANGE;
	}
}

// results possibilities
static Results::Val results[16] = {
// result   P1   P2
	Results::CLOSE,            // ADVN ADVN -> automatic success + subtract 1
	Results::CLOSE_ROLL_P1,    // ADVN MNTN -> roll  for success + oppose p1 rush
	Results::CLOSE,            // ADVN HOLD -> automatic success + subtract 1
	Results::CLOSE_ROLL_P1,    // ADVN RTRT -> roll  for success + oppose p1 rush
   Results::CLOSE_ROLL_P2,    // MNTN ADVN -> roll  for success + oppose p2 rush
	Results::NO_CHANGE,        // MNTN MNTN -> automatic success + no move
	Results::NO_CHANGE,        // MNTN HOLD -> automatic success + no move
	Results::WITHDRAW_ROLL_P2, // MNTN RTRT -> roll  for success + oppose p2 rush
   Results::CLOSE,            // HOLD ADVN -> automatic success + subtract 1
	Results::NO_CHANGE,        // HOLD MNTN -> automatic success + no move
	Results::NO_CHANGE,        // HOLD HOLD -> automatic success + no move
	Results::WITHDRAW,         // HOLD RTRT -> automatic success + add 1
   Results::NO_CHANGE,        // RTRT ADVN -> automatic success + no move
	Results::WITHDRAW_ROLL_P1, // RTRT MNTN -> roll  for success + oppose p1 rush
	Results::WITHDRAW,         // RTRT HOLD -> automatic success + add 1
	Results::WITHDRAW          // RTRT RTRT -> automatic success + add 1
};

//--------------------- range, the distance between the fighters
void Range::update(int p1, int p2, Ship  &s1, Ship  &s2)
{
//const int oldr = cur_;

   const int moves = s1.move() + s2.move();
//if(moves == 0) printf("What?????!\n");
//fflush(stdout);

   --p1;
   --p2;
   Results::Val result = results[((p1 & 3) << 2) | (p2 & 3)];

	if( Results::needRoll(result) )
	{
		const bool p1Success = rand() % 100 < ((s1.move() / (double)moves) * 100);
		result = Results::resolveRoll(result, p1Success);
	}

	const int upd = Results::delta(result);
	if( !upd ) return;

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

