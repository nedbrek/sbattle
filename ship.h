#ifndef SHIP_H
#define SHIP_H

#include <list>

struct Hit;
class  Range;
struct Ship;

// any one weapon
struct Weapon
{
   int damage;
   int hit;
};

// a ship
struct Ship
{
public:
   Ship(char t);
// Ship(istream &i);

   ~Ship(void)
   {
      delete[] name;
   }

   void reset(void);

   int dead(void);
   int alive(void);
   int plan(Range &r);

   int fight(Ship  &w);

   void repair(void);
   void fire(Range &r, Hit *ghit);
   void damage(Hit *dam);

   void destroy_weapon(int which);

	int move(void) const { return move_; }

protected:
   void evalFire(Range &r, Ship &s2);
// void serialIn(istream &i);

   char    race;
   char   *name;
   int     move_;

	// current
   int     numWeapons;
   Weapon *weapons;

   int     shields;
   int     hull;
};

#endif

