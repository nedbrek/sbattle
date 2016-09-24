#ifndef SIM_H
#define SIM_H
// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles
// header file, definitions and classes

#include "ship.h"
#include <list>

typedef std::list<Ship> Force;

//#define DEBUG
#ifndef NULL
#define NULL 0
#endif

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
      if( t == 0 ) ++impsWin;
      if( t == 1 ) ++bugsWin;
   }

	void print(int numTrials) const;
};

extern Analysis a;

/// stack of damage packets
struct Hit
{
protected:
   int len;
   int *damage;

public: // data
   int number;

public:
   Hit(int ct = 0): len(ct), damage(NULL), number(0)
   {
      if( ct ) damage= new(int[ct]);
   }
//   Hit(const Hit &h);

   ~Hit(void)
   {
      delete[] damage;
   }

	/// add or remove a damage packet
	void add(int d);
   void remove(void);

	/// get the current damage packet
	int  read(void) const;
};

class Battle
{
protected:
	std::list<Force> forces;

public:
	Battle(void);

	void initDefault(void);

   void reset(void);

	///@return 0 Imps/1 Bugs/else draw
	int fight(void);
};

#endif

