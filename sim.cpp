// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles

#include "sim.h"
#include <ctime>

#ifdef DEBUG
#define TRIALS 1
#else
#define TRIALS 5000
#endif

//----------------------------------------------------------------------------
Analysis a;

void Analysis::print(void)
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

//----------------------------------------------------------------------------
void setup(char *f)
{
}

int main(int argc, char **argv)
{
   if( argc != 2 && argc != 3 )
	{
      printf("Usage: %s [--test] <battleFile>\n", argv[0]);

   	return 1;
	}

	if( strcmp(argv[1], "--test") != 0 )
	{
   	time_t ti;
   	srand((unsigned) time(&ti));

   	setup(argv[1]);
	}

	Battle theBattle;

	for(int i = 0; i < TRIALS; i++)
   {
#ifdef DEBUG
printf("Trial:%d", i);
#endif
      a.round(theBattle.fight());
#ifdef DEBUG
printf("\nEnd:%d\n\n", t);
#endif
      theBattle.reset();
   }
   a.print();

   return(0);
}
