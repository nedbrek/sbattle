// Edward Brekelbaum
// March 16, 1998 - May 3, 1998
// Simulation of space battles

#include "sim.h"

Battle theBattle;
Analysis a;

void setup(char *f)
{
}

int main(int argc, char **argv)
{
   time_t ti;
   srand((unsigned) time(&ti));

   if(argc != 2)
      printf("Usage: %s <battleFile>\n", argv[0]);

   setup(argv[1]);

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
