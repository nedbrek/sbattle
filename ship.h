#ifndef SHIP_H
#define SHIP_H

#include <list>

struct Hit;
class  Range;

//----------------------------------------------------------------------------
/// any one weapon
struct Weapon
{
   int damage; /// damage done when it hits
   int hit;    /// hit probability
};

// Range actions
const int RANGE_ADVN= 1; // advance
const int RANGE_MNTN= 2; // maintain current range (counter-maneuver)
const int RANGE_HOLD= 3; // hold position (don't move)
const int RANGE_RTRT= 4; // retreat/disengage

/// a ship
struct Ship
{
protected:
   char    race;
   char   *name;
   int     move_;

	// current
   int     numWeapons;
   Weapon *weapons;

   int     shields;
   int     hull;

protected: // methods
   void evalFire(Range &r, Ship &s2);
// void serialIn(istream &i);

public:
    Ship(char t);
//  Ship(istream &i);
	 // deep copy
	 Ship(const Ship &b);
	 Ship& operator=(const Ship &b);
   ~Ship(void);

   void reset(void);

	///@return an integer code: negative numbers for various failure modes
	/// 0 for alive
   int  dead (void) const;

	///@return true if ship is still in action
	bool alive(void) const;

	///@return a range action as the movement strategy
	int plan(Range &r) const;

	/// fight this ship against w
	///@return -1/0/1 as draw/opponent/we win
   int fight(Ship &w);

   void repair(void);
   void fire(Range &r, Hit *ghit);
   void damage(Hit *dam);

   void destroy_weapon(int which);

	int move(void) const { return move_; }
};

#endif

