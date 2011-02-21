// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles

#include "sim.h"

//----------------------------------------------------------------------------
Analysis a;

void Analysis::print(int numTrials) const
{
	printf("Imps: %d \t Bugs: %d, Draws: %d\n", 
	         impsWin, bugsWin, numTrials - impsWin - bugsWin);
	printf("Imps lost due to hull failure: %d\n", impsNoHull);
	printf("Imps lost due to weapons failure: %d\n", impsNoWeap);
	printf("Bugs lost due to hull failure: %d\n", bugsNoHull);
	printf("Bugs lost due to weapons failure: %d\n", bugsNoWeap);
	printf("Bugs lost due to weapons unable to breach enemy shields: %d\n", 
	    impsShieldRegen);
	printf("Total number of rounds fought:%d Average: %f\n", sumRounds,
	    sumRounds/(double)numTrials);
}

