/* Main File for CPU Context Switch Program - Project 0 (CS531)
 * Do Not Modify this File 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sys.h"

/* Local Function Prototypes */
static int parse_seed(const char *argument);

/* Takes one argument: Test Number to Run */
int main(int argc, char *argv[]) {
  int random_seed = 1;

  /* Check for Command Line Arguments (2) */
  if(argc > 2) {
    printf("Usage: %s [test_number]\n", argv[0]);
    exit(0);
  }
  /* Parse if present, else allow default value to remain */
  else if(argc == 2) {
    random_seed = parse_seed(argv[1]);
  }

  /* Seed the Random Number Generator to generate new Test Data */
  srand(random_seed);
  
  /* Initialize the Scheduler and Run */
  sys_init(random_seed);
  sys_run();

  return 0;
}

/* Parse a CLI argument into an unsigned int for the seed. */
static int parse_seed(const char *argument) {
  unsigned int seed = 0;
  errno = 0;
  seed = strtol(argument, NULL, 10);
  if(errno != 0) {
    printf("Error parsing the test_number, exiting.\n");
    exit(-1);
  }
  return seed;
}
