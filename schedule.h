/* Do Not Modify This File */
#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "structs.h"

int schedule_count(Process *list);
void schedule_insert(Process **list, Process *node);
void schedule_terminate(Process *node);
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run);
Process *schedule_select(Process **list);

#endif /* SCHEDULE_H */
