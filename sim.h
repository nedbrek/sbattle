#ifndef SIM_H
#define SIM_H
// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// header file, definitions and classes

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>

#include <list>

//#define DEBUG
#ifdef DEBUG
#define TRIALS 1
#else
#define TRIALS 5000
#endif

// Range values
#define RANGE_PTBK 1
#define RANGE_SHRT 2
#define RANGE_MEDM 3
#define RANGE_LONG 4
#define RANGE_EXTR 5

// Range actions
#define RANGE_ADVN 1
#define RANGE_MNTN 2
#define RANGE_HOLD 3
#define RANGE_RTRT 4

struct Ship;

struct Analysis
{
   int bugsWin;
   int impsWin;
   int bugsNoHull;
   int impsNoHull;
   int bugsNoWeap;
   int impsNoWeap;
   int impsShieldRegen;
   int sumRounds;

   Analysis(void)
   {
      reset();
   }

   void reset(void)
   {
      impsWin= 0;
      bugsWin= 0;
      bugsNoHull= 0;
      impsNoHull= 0;
      bugsNoWeap= 0;
      impsNoWeap= 0;
      impsShieldRegen= 0;
      sumRounds= 0;
   }

   void round(int t)
   {
      if( t == 0 ) impsWin++;
      if( t == 1 ) bugsWin++;
   }

   void print(void)
   {
      printf("Imps: %d \t Bugs: %d, Draws: %d\n", 
              impsWin, bugsWin, TRIALS - impsWin - bugsWin);
      printf("Imps lost due to hull failure: %d\n", impsNoHull);
      printf("Imps lost due to weapons failure: %d\n", impsNoWeap);
      printf("Bugs lost due to hull failure: %d\n", bugsNoHull);
      printf("Bugs lost due to weapons failure: %d\n", bugsNoWeap);
      printf("Bugs lost due to weapons unable to breach enemy shields: %d\n", 
              impsShieldRegen);
      printf("Total number of rounds fought:%d Average: %f\n", sumRounds,
             sumRounds/(double)TRIALS);
   }
};

extern Analysis a;

// effective range of all ships
struct Range
{
   int cur;
   int mod;

   // start at extreme
   Range(void): cur(RANGE_EXTR), mod(-40) {}
   // move according to plans
   void update(int p1, int p2, Ship &s1, Ship &s2);
};

// any one weapon
struct Weapon
{
   int damage;
   int hit;
};

// stack of damage packets
struct Hit
{
protected:
   int *damage;

public:
   int number;
   int len;

   void add(int d);
   void remove(void);
   int  read(void);

   Hit(int ct = 0): damage(NULL), number(0), len(ct)
   {
      if( ct ) damage= new(int[ct]);
   }
//   Hit(const Hit &h);

   ~Hit(void)
   {
      delete[] damage;
   }
};

// a ship
struct Ship
{
   char race;
   char *name;
   int numWeapons;
   Weapon *weapons;
   int shields;
   int hull;
   int move;

   ~Ship()
   {
      delete[] name;
//      if(weapons) delete[] weapons;
   }

   Ship(char t);
   int fight(Ship  &w);
   int dead(void);
   int alive(void);
   int plan(Range  &r);
   void repair(void);
   void fire(Range  &r, Hit *ghit);
   void damage(Hit *dam);
   void destroy_weapon(int which);
   void reset(void);

private:
   void evalFire(Range &r, Ship &s2);
};

typedef std::list<Ship> Force;

struct Battle
{
	std::list<Force> forces;
   Analysis a;

   void reset(void);

	///@return 0 Imps/1 Bugs/else draw
	int fight(void);
};

#endif

