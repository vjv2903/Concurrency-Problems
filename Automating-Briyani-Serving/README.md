# AUTOMATING BIRIYANI SERVING

## LOGIC AND IMPLEMENTATION: CODE EXPLANATION

The user provides the values of the number of cooks, tables and students. Then the simulation starts. Arrays of pointers are created, one each for cooks, tables and students. First the memory is allocated to these structure pointers and their values are initialised. Then threads are created, one each for each cook, table and student. Only the student threads are joined as once all students are done, the simulation should be over.

### COOKS

At the start, the cooks are initialised with status JOBLESS. First, the cook invokes prepare_biryani() where its state changes to COOKING as it prepares a random number of vessels in a random time interval. Then its status changes to COOK_WAITING. Then it calls biryani_ready() where it waits till it has transferred all the vessels made to the tables. To check/modify the state of the table, we use mutex locks corresponding to that table.

### TABLES

All the tables are initialised with state WAITING with no slots available or left in the vessel. The function wait_for_vessel() returns iff the table has a vessel in its container. On returning, it invokes a ready_to_serve_table(), which returns once all the slots in the vessel and those available are empty. It also takes care about making random number of slots available in case all the available slots were eaten up by the students. Here, we use table mutex to lock the table values while changing/modifying its details.

### STUDENTS

All the students are initialised with state NOT_ARRIVED. Initially sleep(arrival_time) to simulate the random arrival times of the students. Then once the student arrives, change his state to WAIT4SLOT and invoke wait_for_slot() where the student checks for free slots in random tables. If the table has free slots then, the student occupies the slot and then invokes student_in_slot() and gets served and they leave the system.

### MAIN FUNCTION

Once all students are done, we clear all the memory allocated for cooks, students and tables.

## HOW TO RUN

> gcc -pthread q2_code.c
> ./a.out

## SAMPLE RUN

 Enter the number of :
        Robot cooks:4
        Serving tables:5
        Students:19

 Simulation begins  
 Robot Chef 1 is preparing 5 vessels of Biryani  
 Robot Chef 2 is preparing 10 vessels of Biryani  
 Robot Chef 3 is preparing 4 vessels of Biryani  
 Student 14 has arrived  
 Student 14 is waiting to be allocated a slot on the serving table  
 Robot Chef 0 is preparing 5 vessels of Biryani  
 Student 13 has arrived  
 Student 13 is waiting to be allocated a slot on the serving table  
 Student 8 has arrived  
 Student 8 is waiting to be allocated a slot on the serving table  
 Robot Chef 3 has prepared 4 vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking  
 Serving Container of Table 0 is refilled by Robot Chef 3; Table 0 resuming serving now  
 Serving Container of Table 1 is refilled by Robot Chef 3; Table 1 resuming serving now  
 Student 4 has arrived  
 Student 4 is waiting to be allocated a slot on the serving table  
 Student 11 has arrived  
 Student 11 is waiting to be allocated a slot on the serving table  
 Serving Container of Table 2 is refilled by Robot Chef 3; Table 2 resuming serving now  
 Serving table 2 entering Serving Phase  
 Serving Table 2 is ready to serve with 4 slots  
 Student 4 assigned a slot on the serving table 2 and waiting to be served  
 Student 4 on serving table 2 has been served.  
 Student 11 assigned a slot on the serving table 2 and waiting to be served  
 Student 11 on serving table 2 has been served.  
 Student 13 assigned a slot on the serving table 2 and waiting to be served  
 Student 13 on serving table 2 has been served.  
 Serving table 0 entering Serving Phase  
 Serving Table 0 is ready to serve with 5 slots  
 Serving Container of Table 3 is refilled by Robot Chef 3; Table 3 resuming serving now  
 All the vessels prepared by Robot Chef 3 are emptied. Resuming cooking now.  
 Robot Chef 3 is preparing 4 vessels of Biryani  
 Student 8 assigned a slot on the serving table 2 and waiting to be served  
 Student 8 on serving table 2 has been served.  
 Serving table 1 entering Serving Phase  
 Serving Table 1 is ready to serve with 6 slots  
 Serving Table 2 is ready to serve with 5 slots  
 Serving table 3 entering Serving Phase  
 Serving Table 3 is ready to serve with 1 slots  
 Student 14 assigned a slot on the serving table 2 and waiting to be served  
 Student 14 on serving table 2 has been served.  
 Robot Chef 0 has prepared 5 vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking  
 Robot Chef 2 has prepared 10 vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking  
 Serving Container of Table 4 is refilled by Robot Chef 0; Table 4 resuming serving now  
 Serving table 4 entering Serving Phase  
 Serving Table 4 is ready to serve with 6 slots  
 Robot Chef 1 has prepared 5 vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking  
 Student 2 has arrived  
 Student 2 is waiting to be allocated a slot on the serving table  
 Student 2 assigned a slot on the serving table 3 and waiting to be served  
 Student 2 on serving table 3 has been served.  
 Serving Table 3 is ready to serve with 10 slots  
 Student 6 has arrived  
 Student 6 is waiting to be allocated a slot on the serving table  
 Student 6 assigned a slot on the serving table 3 and waiting to be served  
 Student 6 on serving table 3 has been served.  
 Robot Chef 3 has prepared 4 vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking  
 Student 18 has arrived  
 Student 18 is waiting to be allocated a slot on the serving table  
 Student 18 assigned a slot on the serving table 0 and waiting to be served  
 Student 18 on serving table 0 has been served.  
 Student 5 has arrived  
 Student 5 is waiting to be allocated a slot on the serving table  
 Student 5 assigned a slot on the serving table 1 and waiting to be served  
 Student 5 on serving table 1 has been served.  
 Student 9 has arrived  
 Student 9 is waiting to be allocated a slot on the serving table  
 Student 9 assigned a slot on the serving table 3 and waiting to be served  
 Student 9 on serving table 3 has been served.  
 Student 17 has arrived  
 Student 17 is waiting to be allocated a slot on the serving table  
 Student 17 assigned a slot on the serving table 2 and waiting to be served  
 Student 17 on serving table 2 has been served.  
 Student 16 has arrived  
 Student 16 is waiting to be allocated a slot on the serving table  
 Student 16 assigned a slot on the serving table 1 and waiting to be served  
 Student 16 on serving table 1 has been served.  
 Student 10 has arrived  
 Student 10 is waiting to be allocated a slot on the serving table  
 Student 10 assigned a slot on the serving table 3 and waiting to be served  
 Student 10 on serving table 3 has been served.  
 Student 1 has arrived  
 Student 1 is waiting to be allocated a slot on the serving table  
 Student 1 assigned a slot on the serving table 1 and waiting to be served  
 Student 1 on serving table 1 has been served.  
 Student 12 has arrived  
 Student 12 is waiting to be allocated a slot on the serving table  
 Student 12 assigned a slot on the serving table 4 and waiting to be served  
 Student 12 on serving table 4 has been served.  
 Student 7 has arrived  
 Student 7 is waiting to be allocated a slot on the serving table  
 Student 7 assigned a slot on the serving table 4 and waiting to be served  
 Student 7 on serving table 4 has been served.  
 Student 0 has arrived  
 Student 0 is waiting to be allocated a slot on the serving table  
 Student 0 assigned a slot on the serving table 1 and waiting to be served  
 Student 0 on serving table 1 has been served.  
 Student 3 has arrived  
 Student 3 is waiting to be allocated a slot on the serving table  
 Student 3 assigned a slot on the serving table 0 and waiting to be served  
 Student 3 on serving table 0 has been served.  
 Student 15 has arrived  
 Student 15 is waiting to be allocated a slot on the serving table  
 Student 15 assigned a slot on the serving table 1 and waiting to be served  
 Student 15 on serving table 1 has been served.  
 Simulation Over.
