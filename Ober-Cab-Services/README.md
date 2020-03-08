
# OBER CAB SERVICES

Given N cabs, M riders and K payment servers you need to implement a working system which ensures correctness and idempotency. Each cab has one driver. In Ober, payments by the riders should be done in Payment Servers only. Ober provides two types of cab services namely pool and premier ride. In pool ride maximum of two riders can share a cab whereas in premier ride only one rider. There are four states for a cab namely waitState (no rider in cab), onRidePremier, onRidePoolFull (pool ride with two riders), onRidePoolOne (pool ride with only one rider).

## INPUT

Number of cabs, riders and servers

## LOGIC AND IMPLEMENTATION

All the riders are considered as structures. At the start, the riders are initialised with status free_state. Then, we create one thread per rider.

Each rider takes a random interval of time to arrive. Once the rider arrives, he waits for a suitable cab depending on the type of cab he wants to book.  As long as the flag is zero, we would keep checking the time elapsed and also keep on checking for more cabs. If the time elapsed exceeds the maximum time, the rider quits the system.

When the rider finds a suitable driver, the mutex associated to the driver is locked, driver details are changed and then unlocked.

Then the thread sleeps for the duration of the ride time.  Once the ride is done, the mutex associated to the driver is locked, driver details are changed accordingly and then unlocked. the state of the rider now becomes wait_for_pay.

Then the rider searches for a free server to do payment.If the server is free, then it is made busy and the payment is initiated. Once that is done, the server becomes free and the rider status becomes payment_done. Then the rider leaves the system.

### MAIN FUNCTION

Create arrays of rider pointers, driver pointers and server pointers and create the rider threads. Join only the rider threads as we want the simulation to end once all the riders leave the system. Then we clear all the memory allocated for cooks, students and tables.

## HOW TO RUN

> gcc -pthread q3_code.c
> ./a.out

## SAMPLE RUN

Enter the number of:  
         Drivers:       5  
         Servers:       4  
         Riders:        20  
  
Rider 6: arrived  
Rider 6: Found driver 0  
Rider 14: arrived  
Rider 14: Found driver 1  
Rider 4: arrived  
Rider 4: Found driver 2  
Rider 17: arrived  
Rider 17: Found driver 3  
Rider 15: arrived  
Rider 15: Found driver 4  
Rider 10: arrived  
Rider 10: Found driver 4  
Rider 3: arrived  
Rider 8: arrived  
Rider 16: arrived  
Rider 0: arrived  
Rider 8: Time Out. Hence exiting the system  
Rider 3: Time Out. Hence exiting the system  
Rider 18: arrived  
Rider 0: Time Out. Hence exiting the system  
Rider 1: arrived  
Rider 10: Ride over (4)  
Rider 18: Found driver 4  
Rider 15: Ride over (4)  
Rider 10: Found server 0  
Rider 14: Ride over (1)  
Rider 1: Found driver 1  
Rider 14: Found server 1  
Rider 15: Found server 2  
Rider 4: Ride over (2)  
Rider 16: Found driver 2  
Rider 4: Found server 3  
Rider 6: Ride over (0)  
Rider 17: Ride over (3)  
Rider 10: Payment_done (0)  
Rider 6: Found server 0  
Rider 14: Payment_done (1)  
Rider 17: Found server 1  
Rider 15: Payment_done (2)  
Rider 13: arrived  
Rider 13: Found driver 0  
Rider 4: Payment_done (3)  
Rider 5: arrived  
Rider 5: Found driver 3  
Rider 6: Payment_done (0)  
Rider 17: Payment_done (1)  
Rider 9: arrived  
Rider 9: Found driver 3  
Rider 12: arrived  
Rider 7: arrived  
Rider 19: arrived  
Rider 11: arrived  
Rider 11: Found driver 4  
Rider 2: arrived  
Rider 16: Ride over (2)  
Rider 16: Found server 0  
Rider 2: Found driver 2  
Rider 16: Payment_done (0)  
Rider 12: Time Out. Hence exiting the system  
Rider 18: Ride over (4)  
Rider 18: Found server 0  
Rider 13: Ride over (0)  
Rider 13: Found server 1  
Rider 19: Found driver 0  
Rider 1: Ride over (1)  
Rider 1: Found server 2  
Rider 7: Found driver 1  
Rider 9: Ride over (3)  
Rider 9: Found server 3  
Rider 18: Payment_done (0)  
Rider 13: Payment_done (1)  
Rider 1: Payment_done (2)  
Rider 9: Payment_done (3)  
Rider 5: Ride over (3)  
Rider 5: Found server 0  
Rider 2: Ride over (2)  
Rider 2: Found server 1  
Rider 2: Payment_done (1)  
Rider 5: Payment_done (0)  
Rider 11: Ride over (4)  
Rider 11: Found server 0  
Rider 11: Payment_done (0)  
Rider 7: Ride over (1)  
Rider 7: Found server 0  
Rider 19: Ride over (0)  
Rider 19: Found server 1  
Rider 7: Payment_done (0)  
Rider 19: Payment_done (1)  
