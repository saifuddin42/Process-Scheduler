#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"


//Prototyping of two extra functions I wrote:
Process *check_starvation(Process **list, Process *temp);
void adjust_schedule(Process **list, Process *temp);


/* Schedule Count
 * - Return the number of Processes in the list.
 */
int schedule_count(Process *list) {
 
	//Counter Initialization.
	int count = 0;

	//If list doesn't exist, return 0.
	if(list == NULL) { 
		return count;
	}

	//Else count the number or nodes in the list and return them.
	else {
		
		Process *walker = list;

		while(walker != NULL) {
			count ++;
			walker = walker->next;
		}

		return count;
	}
}


/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember to set list to the new head if it changes!
 */
void schedule_insert(Process **list, Process *node) {

	//If node doesn't exist, return as there is nothing to insert.
	if(node == NULL) {
		return;
	}

	//Else proceed further.
	else { 

		Process *walker = *list;

		//If the list is empty or if the new node to insert should be inserted before the list:
		if (*list == NULL || (*list)-> pid >= node -> pid) { 

			node -> next = *list; 
			*list = node;
		}

		//Else insert the node either midway or at the end of the list.
		else { 
    
			//Travel the list till walker reaches the final node or just before a node 
			//whose pid is greater than the new node's pid.
			while ((walker -> next != NULL) && ((walker -> next -> pid) < (node -> pid))) { 
      
				walker = walker -> next;
			} 
            
			//Insert node (Works for both of the above cases).
			node -> next = walker -> next; 
			walker -> next = node; 
		}
	}
}


/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {
  
	//If the node to terminate doesn't exist, return.
	if(node == NULL) {
		return;
	}
    
	//Else free the node and set the pointer pointing to it to NULL.
	else {
		free(node);
		node = NULL;
	}
}


/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {

	//Create empty node by taking memory from malloc.
	Process *node = malloc(sizeof(Process));

	//If there is any error, return NULL.
	if(node == NULL) {
		return NULL;
	}

	//Else generate node.
	else {
		strncpy(node -> name, name, strlen(name) + 1);
		node -> pid = pid;
		node -> time_remaining = time_remaining;
		node -> time_last_run = time_last_run;
		node -> next = NULL;
		return node;
	}
}


/* Schedule Select
 * - Select the next Process to be run using the following conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *      (Use the function clock_get_time() to get the current time)
 *   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */
Process *schedule_select(Process **list) {

	Process *temp = *list;
	Process *walker = *list;
	//this will be used for getting the node with either least time remaining or the starving node.
	Process *check = NULL;

	//If list doesn't exist then return NULL.
	if(*list == NULL) { 
		return NULL;
	}

	else {

		//traverse list using walker to check lowest time_remaining.
		while (walker -> next != NULL) {

			walker = walker -> next;

			//if the walker's time_remaining is less than temp's time_remaining, 
			//then update temp with walker's time_remaining
			if(temp -> time_remaining > walker -> time_remaining) { 
				//this will ensure that temp always points to the node with lowest time 
				//remaining and in case of conflict the current temp will always have 
				//lowest pid since our list is already sorted.
				temp = walker;
			}
		}

		//calling custom function to either return the same temp or in case of a starving node return that node.
		check = check_starvation(list, temp);

		//calling custom function to adjust the process list after the selection is done.
		adjust_schedule(list, check);

		//return selected node.
		return check;
	}
}


/* Schedule Check Starvation
 * - Check the list for any starving nodes.
 * - If there is no starving node,
 *     Return the original node passed to the function which is the node containing lowest time_remaining.
 * - Else
 *     Return the new starving node.
 */
Process *check_starvation(Process **list, Process *temp) {

	Process *walker = *list;
	Process *prewalker = *list;
	Process *temp2 = NULL;

	//traverse list using walker to get the first starving node.
	while (walker -> next != NULL) {
      
		walker = walker -> next;

		//This code ensures that temp2 always points to the node with first 
		//starving node which also ensures that temp2 will always have 
		//lowest pid since our list is already sorted.
		if((clock_get_time() - (prewalker -> time_last_run)) >= TIME_STARVATION) { 
      
			//if prewalker's starvation time is greater than or equal to TIME_STARVATION, 
			//then set temp2 = prewalker
			temp2 = prewalker;
			break;
		}

		//This IF block checks the starvation time for the last remaining node
		if(walker -> next == NULL) {

			if((clock_get_time() - (walker -> time_last_run)) >= TIME_STARVATION) { 
        
				//if walker's starvation time is greater than or equal to TIME_STARVATION, 
				//then set temp2 = walker
				temp2 = walker;
				break;
			}
		}

		prewalker = prewalker -> next;
	}

	//if temp2 is still null that means there is no starving node so we return the original temp itself.
	if(temp2 == NULL) {
		return temp;
	}
	else {
		return temp2;
	}
}


/* Schedule Adjust
 * - Check the list for the selected node (temp).
 * - Adjust the list such that the node before temp points to the node after temp or NULL depending on the scenario. 
 */
void adjust_schedule(Process **list, Process *temp) {

	Process *walker = *list;
	Process *prewalker = *list;

	//If list doesn't exist, return.
	if(*list == NULL) {
		return;
	}

	//Else if either temp is the first node of the list or temp is the only node in the list.
	else if(walker -> pid == temp -> pid) {
		//sets list as the next node or NULL depending on one of the above cases.
		*list = walker -> next;
		return;
	}

	//Else traverse the list till you find that particular node and adjust accordingly.
	else {

		while(walker -> next != NULL) {

			walker = walker -> next;

			//When correct node is found, adjust its predecessor accordingly.
			//This will work for both cases if the node to be adjusted is 
			//found in the middle of the list or the end of the list.
			if(walker -> pid == temp -> pid) {
				prewalker -> next = walker -> next;
				return;
			}

			prewalker = prewalker -> next;
		}
	}
}