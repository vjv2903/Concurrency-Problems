
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#define _POSIX_C_SOURCE 199309L
#define waitState 0
#define onRidePremier 1
#define onRidePoolFull 2
#define onRidePoolOne 3
#define busy 1
#define free_state 0
#define waiting 0
#define inCab 1
#define wait_for_pay 2
#define in_payment 3
#define payment_done -1
#define random_integer(X, Y) random() % (Y - X + 1) + X
#define makezero(X) X = 0
int number_of_drivers, number_of_servers, number_of_riders;
typedef struct Driver
{
    int in, state, type, no_of_passengers, Rider_in_cab[2];
    pthread_mutex_t mutex;
} Driver;
typedef struct Rider
{
    pthread_t tid;
    int in, state, type, arrivalTime, maxWaitTime, RideTime, Driver_of_cab, Server_to_pay;
} Rider;
typedef struct Server
{
    int in, state, Rider_in_payment;
    pthread_t tid;
    pthread_mutex_t mutex;
} Server;
Driver **drivers;
Rider **riders;
Server **servers;
int BookCab(Rider **);
void makePayment(Rider **Ri);
void *Rider_thread(void *arg);
void *Rider_thread(void *arg)
{
    Rider *R = (Rider *)arg;
    sleep(R->arrivalTime);
    printf("\nRider %d: arrived", R->in);
    if (BookCab(&R) == -1)
    {
        R->state = payment_done;
        return NULL;
    }
    makePayment(&R);
    return NULL;
}
int BookCab(Rider **Ri)
{
    Rider *R = *Ri;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double start_time = ts.tv_nsec / (1e9) + ts.tv_sec, end_time;
    int flag, i;
    makezero(flag);
    while (!flag)
    {
        makezero(flag);
        Ri = &R;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        end_time = ts.tv_nsec / (1e9) + ts.tv_sec;
        if (end_time - start_time > R->maxWaitTime)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            printf("\nRider %d: Time Out. Hence exiting the system", R->in);
            R->state = payment_done;
            Ri = &R;
            flag = 1;
            return -1;
        }
        makezero(i);
        while (i < number_of_drivers)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            end_time = ts.tv_nsec / (1e9) + ts.tv_sec;
            if (end_time - start_time > R->maxWaitTime)
            {
                printf("\nRider %d: Time Out. Hence exiting the system", R->in);
                R->state = payment_done;
                Ri = &R;
                return -1;
            }
            pthread_mutex_lock(&(drivers[i]->mutex));
            if (drivers[i]->state == waitState)
            {
                printf("\nRider %d: Found driver %d", R->in, i);
                if (R->type == 1)
                    drivers[i]->state = onRidePremier;
                else
                    drivers[i]->state = onRidePoolOne;
                drivers[i]->type = R->type;
                makezero(flag);
                drivers[i]->Rider_in_cab[0] = R->in;
                drivers[i]->no_of_passengers = 1;
                R->state = inCab;
                R->Driver_of_cab = i;
                makezero(flag);
                pthread_mutex_unlock(&(drivers[i]->mutex));
                flag = 1;
                Ri = &R;
                break;
            }
            else if (drivers[i]->state == onRidePoolOne && !R->type)
            {
                printf("\nRider %d: Found driver %d", R->in, i);
                drivers[i]->Rider_in_cab[1] = R->in;
                drivers[i]->state = onRidePoolFull;
                drivers[i]->no_of_passengers = 2;
                makezero(flag);
                R->state = inCab;
                R->Driver_of_cab = i;
                makezero(flag);
                pthread_mutex_unlock(&(drivers[i]->mutex));
                flag = 1;
                Ri = &R;
                break;
            }
            pthread_mutex_unlock(&(drivers[i]->mutex));
            i = i + 1;
        }
    }
    Ri = &R;
    sleep(R->RideTime);
    printf("\nRider %d: Ride over (%d) ", R->in, R->Driver_of_cab);
    int cab_driver_id = R->Driver_of_cab;
    pthread_mutex_lock(&(drivers[cab_driver_id]->mutex));
    if (!(R->type - 1))
    {
        drivers[cab_driver_id]->state = waitState;
        drivers[cab_driver_id]->type = -1;
        makezero(drivers[cab_driver_id]->no_of_passengers);
        makezero(flag);
        drivers[cab_driver_id]->Rider_in_cab[0] = -1;
        R->state = wait_for_pay;
        R->Driver_of_cab = -1;
        Ri = &R;
    }
    else if (!R->type)
    {
        R->state = wait_for_pay;
        R->Driver_of_cab = -1;
        Ri = &R;
        if (drivers[cab_driver_id]->state == onRidePoolOne)
        {
            drivers[cab_driver_id]->state = waitState;
            makezero(flag);
            makezero(drivers[cab_driver_id]->no_of_passengers);
            drivers[cab_driver_id]->type = -1;
            drivers[cab_driver_id]->Rider_in_cab[0] = -1;
        }
        else if (drivers[cab_driver_id]->state == onRidePoolFull)
        {
            drivers[cab_driver_id]->state = onRidePoolOne;
            makezero(drivers[cab_driver_id]->type);
            makezero(flag);
            drivers[cab_driver_id]->Rider_in_cab[1] = -1;
            drivers[cab_driver_id]->no_of_passengers = 1;
            int other_passenger;
            if (drivers[cab_driver_id]->Rider_in_cab[0] == R->in)
                other_passenger = drivers[cab_driver_id]->Rider_in_cab[1];
            else
                other_passenger = drivers[cab_driver_id]->Rider_in_cab[0];
            drivers[cab_driver_id]->Rider_in_cab[0] = other_passenger;
        }
    }
    pthread_mutex_unlock(&(drivers[cab_driver_id]->mutex));
    Ri = &R;
    makezero(flag);
    return 1;
}
void makePayment(Rider **Ri)
{
    Rider *R = *Ri;
    int flag;
    makezero(flag);
    int i;
    while (flag == 0)
    {
        Ri = &R;
        makezero(i);
        while (i < number_of_servers)
        {
            pthread_mutex_lock(&(servers[i]->mutex));
            if (servers[i]->state == free_state)
            {
                makezero(flag);
                printf("\nRider %d: Found server %d", R->in, i);
                servers[i]->state = busy;
                makezero(flag);
                servers[i]->Rider_in_payment = R->in;
                R->state = in_payment;
                makezero(flag);
                R->Server_to_pay = i;
                pthread_mutex_unlock(&(servers[i]->mutex));
                flag = 1;
                Ri = &R;
                break;
            }
            pthread_mutex_unlock(&(servers[i]->mutex));
            i = i + 1;
        }
    }
    Ri = &R;
    int pay_server_id = R->Server_to_pay;
    sleep(2);
    printf("\nRider %d: Payment_done (%d)", R->in, pay_server_id);
    pthread_mutex_lock(&(servers[pay_server_id]->mutex));
    servers[pay_server_id]->state = free_state;
    makezero(flag);
    R->state = payment_done;
    servers[pay_server_id]->Rider_in_payment = -1;
    R->Server_to_pay = -1;
    makezero(flag);
    pthread_mutex_unlock(&(servers[pay_server_id]->mutex));
    Ri = &R;
    return;
}
int main()
{
    srand(time(0));
    int i;
    makezero(i);
    printf("\nEnter the number of:\n");
    printf("\t Drivers: ");
    scanf("%d", &number_of_drivers);
    printf("\t Servers: ");
    scanf("%d", &number_of_servers);
    printf("\t Riders: ");
    scanf("%d", &number_of_riders);
    drivers = (Driver **)malloc(number_of_drivers * sizeof(Driver *));
    riders = (Rider **)malloc(number_of_riders * sizeof(Rider *));
    servers = (Server **)malloc(number_of_servers * sizeof(Server *));
    makezero(i);
    while (i < number_of_riders)
    {
        riders[i] = (Rider *)malloc(sizeof(Rider));
        riders[i]->in = i;
        riders[i]->state = free_state;
        riders[i]->type = random_integer(0, 1);
        riders[i]->maxWaitTime = random_integer(5, 10);
        riders[i]->RideTime = random_integer(10, 20);
        int k = number_of_riders * 1.5;
        riders[i]->arrivalTime = random_integer(0, k);
        riders[i]->Driver_of_cab = riders[i]->Server_to_pay = -1;
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_drivers)
    {
        drivers[i] = (Driver *)malloc(sizeof(Driver));
        drivers[i]->in = i;
        drivers[i]->state = waitState;
        drivers[i]->type = -1;
        drivers[i]->no_of_passengers = 0;
        drivers[i]->Rider_in_cab[0] = drivers[i]->Rider_in_cab[1] = -1;
        pthread_mutex_init(&(drivers[i]->mutex), NULL);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_servers)
    {
        servers[i] = (Server *)malloc(sizeof(Server));
        servers[i]->in = i;
        servers[i]->state = waiting;
        servers[i]->Rider_in_payment = -1;
        pthread_mutex_init(&(servers[i]->mutex), NULL);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_riders)
    {
        pthread_create(&(riders[i]->tid), NULL, Rider_thread, (void *)riders[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_riders)
    {
        pthread_join(riders[i]->tid, NULL);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_riders)
    {
        free(riders[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_drivers)
    {
        pthread_mutex_destroy(&drivers[i]->mutex);
        free(drivers[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < number_of_servers)
    {
        pthread_mutex_destroy(&servers[i]->mutex);
        free(servers[i]);
        i = i + 1;
    }
    free(riders);
    free(servers);
    free(drivers);
    return 0;
}