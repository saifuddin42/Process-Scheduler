/* Do Not Modify this File */

/* Definitions for Structs for this Project */
#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct process_struct {
  char name[255]; /* Process Name */
  int pid; /* Process ID (unique) */
  int time_remaining; /* Time Units Left to Execute */
  int time_last_run; /* Last Time Executed (or time created) */
  struct process_struct *next; 
} Process;

#endif /* STRUCTS_H */
