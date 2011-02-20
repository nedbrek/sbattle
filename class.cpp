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


void nedprint(unsigned int mask)
{
   int i;
   for(i=16; i > 0; i--)
   {
      if(mask & 32768) printf("1");
      else printf("0");
      mask <<= 1;
   }
}

//--------------------- class hit, a stack of damage packets
// read, return top of stack
inline int Hit::read(void)
{
   return(damage[number-1]);
}

// remove, pop off stack
inline void Hit::remove(void)
{
   number--;
   damage[number]=0xdead;
}

// add, push onto stack
inline void Hit::add(int d)
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


//--------------------- class ship, the fighters
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
   if(weapons) delete[] weapons;
   if(name) delete[] name;
   switch(race)
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
            move = BUG_MOVE;
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
            weapons[3].damage = 2;
            weapons[0].hit = 75;
            weapons[1].hit = 75;
            weapons[2].hit = 0;
            weapons[3].hit = 65;
            shields = IMP_S_MAX;
            hull = IMP_H_MAX;
            move = IMP_MOVE;
         break;
   }
}

// place damage packets into ghit for this range
void Ship::fire(Range  &r, Hit *ghit)
{
   int i, hits=0, w;
   unsigned int hitMask=0;

   // for each weapon
   for(i = 0; i < numWeapons; i++)
   {
     hitMask <<= 1;
     if(rand() % 100 < (weapons[i].hit + r.mod()))
     {
        hits++;
        // set this weapon's hit bit to on
        hitMask |= 1;
     }
   }
#ifdef DEBUG
printf("Damage:%d %d ", numWeapons, hits);
nedprint(hitMask);
printf("\t");
#endif

//   Hit ret_val(hits);

   // build damage packets
   i = 0;
   for(w = numWeapons - 1; i < hits; w--)
   {
      // find weapons that hit from armament mask
      if(hitMask & 1)
      {
//         ret_val.damage[i] = weapons[w].damage;
         // add damage packet
         ghit->add(weapons[w].damage);
         i++;
if(weapons[w].damage > 5)
printf("Built: %d %d %d", w, i, weapons[w].damage);
      }
      hitMask >>= 1;
   }

//   return(ret_val);
}

// remove weapon 'which' from this ship's armament
void Ship::destroy_weapon(int which)
{
#ifdef DEBUG
printf("got:%d ", which);
#endif
   weapons[which] = weapons[numWeapons - 1];
   numWeapons--;

/*   if(numWeapons > 0)
   {
      Weapon *t = new(Weapon[numWeapons]);
      for(int i = 0 ; i < numWeapons; i++)
         t[i] = weapons[i];

      delete[] weapons;
      weapons = t;
   }
*/
}

// perform end of round adjustments
void Ship::repair(void)
{
   switch(race)
   {
      case 'I':
      case 'i':
         shields++;
         if(shields > IMP_S_MAX) shields = IMP_S_MAX;
         break;
   }
//printf("Fixa\n");
}

// apply damage to this ship
void Ship::damage(Hit *dam)
{
   int hullMax, i;
   double t;
   int crit;

   // find extent of hull damage
   switch(race)
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
   t = hull / (double)hullMax;
   if(t <= .5)       crit = 30;
   else if(t <= .75) crit = 20;
   else              crit = 0;

//if( race == 'B' ) crit -= 10;

	// assign each damage packet in turn
   for(i = dam->number; i > 0; i--)
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
      if(shields) shields -= dam->read();
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

//--------------------- range, the distance between the fighters
void Range::update(int p1, int p2, Ship  &s1, Ship  &s2)
{
   int moves = s1.move + s2.move;
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
      success |= rand() % 100 < ((s1.move / (double)moves) * 100);
      success &= rand() % 100 >= ((s2.move / (double)moves) * 100);
   }
   if(result & !RESULTS_OP1)
   {
      success = 2; // check for double failure
      success |= rand() % 100 < ((s2.move / (double)moves) * 100);
      success &= rand() % 100 >= ((s1.move / (double)moves) * 100);
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

