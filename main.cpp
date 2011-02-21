#include "sim.h"
#include <ctime>

#ifdef DEBUG
#define TRIALS 1
#else
#define TRIALS 5000
#endif

//----------------------------------------------------------------------------
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
	}

	Battle theBattle;
	theBattle.initDefault();

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
   a.print(TRIALS);

   return(0);
}
