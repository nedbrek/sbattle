#ifndef SIM_H
#define SIM_H
// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// header file, definitions and classes

#include "ship.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>

#include <list>

typedef std::list<Ship> Force;

class Range;

//#define DEBUG
#ifdef DEBUG
#define TRIALS 1
#else
#define TRIALS 5000
#endif

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

struct Battle
{
	std::list<Force> forces;
   Analysis a;

   void reset(void);

	///@return 0 Imps/1 Bugs/else draw
	int fight(void);
};

#endif

