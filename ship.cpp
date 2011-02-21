// implementation of ship class

#include "sim.h"
#include "range.h"

// Imperial Destroyer stats
#define IMP_N "Imperial Destroyer"
#define IMP_W_NUM 3
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

void nedprint(unsigned mask)
{
   for(int i=16; i > 0; --i)
   {
      if(mask & 32768) printf("1");
      else printf("0");
      mask <<= 1;
   }
}

//----------------------------------------------------------------------------
Ship::Ship(char t)
{
   race = t;
   name = NULL;
   weapons = NULL;

   reset();
}

// default weapons and armor
void Ship::reset(void)
{
   delete[] weapons;
   delete[] name;

   switch( race )
   {
      case 'B':
      case 'b':
            name = new(char[sizeof(BUG_N)]);
            strcpy(name, BUG_N);
            numWeapons = BUG_W_NUM;
            weapons = new(Weapon[numWeapons]);
            weapons[0].damage = 5;
            weapons[1].damage = 1;
            weapons[2].damage = 1;
            weapons[0].hit = 40;
            weapons[1].hit = 95;
            weapons[2].hit = 95;
            shields = BUG_S_MAX;
            hull = BUG_H_MAX;
            move_ = BUG_MOVE;
         break;

      case 'I':
      case 'i':
            name = new(char[sizeof(IMP_N)]);
            strcpy(name, IMP_N);
            numWeapons = IMP_W_NUM;
            weapons = new(Weapon[numWeapons]);
            weapons[0].damage = 1;
            weapons[1].damage = 1;
            weapons[2].damage = 2;
            weapons[0].hit = 75;
            weapons[1].hit = 75;
            weapons[2].hit = 65;
            shields = IMP_S_MAX;
            hull = IMP_H_MAX;
            move_ = IMP_MOVE;
         break;
   }
}

// place damage packets into ghit for this range
void Ship::fire(Range &r, Hit *ghit)
{
   int i, hits= 0;

   unsigned hitMask= 0;

   // for each weapon
   for(i = 0; i < numWeapons; ++i)
   {
     hitMask <<= 1;
     if( rand() % 100 < (weapons[i].hit + r.mod()) )
     {
        ++hits;

        // set this weapon's hit bit to on
        hitMask |= 1;
     }
   }

#ifdef DEBUG
printf("Damage:%d %d ", numWeapons, hits);
nedprint(hitMask);
printf("\t");
#endif

   // build damage packets
   i = 0;
   for(int w = numWeapons - 1; i < hits; --w)
   {
      // find weapons that hit from armament mask
      if(hitMask & 1)
      {
         // add damage packet
         ghit->add(weapons[w].damage);
         ++i;

if(weapons[w].damage > 5)
printf("Built: %d %d %d", w, i, weapons[w].damage);
      }

      hitMask >>= 1;
   }
}

// remove weapon 'which' from this ship's armament
void Ship::destroy_weapon(int which)
{
#ifdef DEBUG
printf("got:%d ", which);
#endif

   weapons[which] = weapons[numWeapons - 1];
   --numWeapons;
}

// perform end of round adjustments
void Ship::repair(void)
{
   switch(race)
   {
      case 'I':
      case 'i':
         ++shields;
         if( shields > IMP_S_MAX ) shields = IMP_S_MAX;
         break;
   }
//printf("Fixa\n");
}

// apply damage to this ship
void Ship::damage(Hit *dam)
{
   int hullMax;

   // find extent of hull damage
   switch( race )
   {
      case 'B':
      case 'b':
         hullMax = BUG_H_MAX;
         break;
      case 'I':
      case 'i':
         hullMax = IMP_H_MAX;
         break;
   }

   // assign critical hit percentage as a fraction of hull damage
   const double t = hull / (double)hullMax;

   int crit;
   if     ( t <= .50 ) crit = 30; // less than 50% left
   else if( t <= .75 ) crit = 20; // less than 75% left
   else                crit = 0;  // 76-100 no problem

//if( race == 'B' ) crit -= 10;

	// assign each damage packet in turn
   for(int i = dam->number; i > 0; --i)
   {
if(dam->read() < 1 || dam->read() > 5)
{
printf("Damaged: %d %d ", i, dam->read());
printf("Ick %d\n", dam->number);
}
#ifdef DEBUG
printf("Dam: %d\t", dam->read());
#endif

      // apply damage to shields first
      if( shields ) shields -= dam->read();
      else
      {
         hull -= dam->read();
         // hull damage may destroy weapons
         if( ((rand() % 100) < crit) && numWeapons)
         {
#ifdef DEBUG
printf("bang: %d\t", crit);
#endif
            destroy_weapon(rand() % numWeapons);
         }
      }

      dam->remove();
   }

   // check for shield underflow
   if(shields < 0)
   {
      hull += shields;
      shields = 0;
      if( ((rand() % 100) < crit) && numWeapons)
      {
#ifdef DEBUG
printf("bang: %d\t", crit);
#endif
         // hull damage may destroy weapons
         destroy_weapon(rand() % numWeapons);
      }
   }

#ifdef DEBUG
printf("Hull:%d Shields:%d\n",hull, shields);
#endif
}

// plan movement for this turn
int Ship::plan(Range  &r)
{
   switch(race)
   {
      case 'B':
      case 'b':
            // bugs advance
            return(RANGE_ADVN);
//              return(RANGE_RTRT);
      case 'I':
      case 'i':
/*            if(numWeapons < IMP_W_NUM)
            {
               for(int i = 0; i < numWeapons; i++)
                  if(weapons[i].hit >

            }*/
            // imps go to medium
            if(r.cur() == RANGE_EXTR) return(RANGE_ADVN);
            if(r.cur() == RANGE_LONG) return(RANGE_HOLD);
            return(RANGE_MNTN);
   }
   return(-1);
}

// is this ship alive
int Ship::alive(void)
{
   if(race == 'B' && numWeapons < 2 && weapons[0].damage == 1) return(0);
   return((hull > 0) && (numWeapons > 0));
}

#define DEAD_ENEMY_SHIELDS -1
#define DEAD_NO_HULL       -2
#define DEAD_NO_WEAP       -3

///@return code for how we died, 0 if alive
int Ship::dead(void)
{
   int ret_val= 0;

   if(race == 'B' && numWeapons == 1 && weapons[0].damage == 1)
      return(DEAD_ENEMY_SHIELDS);

   if( hull <= 0 )
      return(DEAD_NO_HULL);

   if( numWeapons <= 0 )
      return(DEAD_NO_WEAP);

   return(ret_val);
}

// fight this ship against w
int Ship::fight(Ship  &w)
{
   Range r;
	int round = 0;

   while(!dead() && !w.dead())
   {
#ifdef DEBUG
printf("\n\tRound\n");
#endif
      // adjust range
      r.update(plan(r), w.plan(r), *this, w);

      evalFire(r, w);

      repair();
      w.repair();

		++round;
   }
	a.sumRounds += round;

	int ret_val = -1;
   if(!dead()) ret_val= 1; // we win
   else // cause of death
      if( dead() == DEAD_ENEMY_SHIELDS )
         a.impsShieldRegen++;
      else
      if( dead() == DEAD_NO_HULL )
         a.bugsNoHull++;
      else
      if( dead() == DEAD_NO_WEAP )
         a.bugsNoWeap++;

   if(!w.dead()) ret_val= 0; // opponent wins
   else
      if( w.dead() == DEAD_NO_HULL )
         a.impsNoHull++;
      else
      if( w.dead() == DEAD_NO_WEAP )
         a.impsNoWeap++;

   return ret_val;
}

// use simultaneous fire
void Ship::evalFire(Range  &r, Ship  &s2)
{
   Hit ghit1(100);
   Hit ghit2(100);

#ifdef DEBUG
printf("\n\t\tFire ship1\t");
#endif
//   dam2 = fire(r);
    fire(r, &ghit1);
#ifdef DEBUG
printf("\n\t\tFire ship2\t");
#endif
//   dam1 = s2.fire(r);
    s2.fire(r, &ghit2);
#ifdef DEBUG
printf("\n\t\tboom ship1\t");
#endif
   damage(&ghit2);
#ifdef DEBUG
printf("\n\t\tboom ship2\t");
#endif
   s2.damage(&ghit1);
}

