#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define JOBLESS 0
#define COOKING 1
#define WAITING 0
#define SERVING 1
#define NOT_ARRIVED 0
#define WAIT4SLOT 1
#define INSLOT 2
#define SERVED 3
#define random_integer(X, Y) random() % (Y - X + 1) + X
#define makezero(X) X = 0
typedef struct
{
    int i, state, no_vessels_in_hand, no_slots_in_vessel;
    pthread_t tid;
    pthread_mutex_t mutex;
} Cook;
typedef struct
{
    int i, no_slots_available, no_slots_left, state;
    pthread_t tid;
    pthread_mutex_t mutex;
} Table;
typedef struct
{
    int i, state, arrival_time;
    pthread_t tid;
} Student;

void *Cook_thread(void *args);
void prepare_biryani(Cook *C);
void biryani_ready(Cook *C);
void *Table_thread(void *args);
void wait_for_vessel(Table *T);
void ready_to_serve_table(Table *T);
void *Student_thread(void *args);
void wait_for_slot(Student *S);
void student_in_slot(Student *S, int j);

int no_table, no_cook, no_student;

Cook **cooks;
Table **tables;
Student **students;

void *Cook_thread(void *args)
{
    int key;
    makezero(key);
    Cook *C = (Cook *)args;
    while (!key)
    {
        prepare_biryani(C);
        biryani_ready(C);
        printf("\n All the vessels prepared by Robot Chef %d are emptied. Resuming cooking now.", C->i);
    }
}
void prepare_biryani(Cook *C)
{
    C->state = COOKING;
    int k = random_integer(1, 10);
    printf("\n Robot Chef %d is preparing %d vessels of Biryani", C->i, k);
    sleep(random_integer(2, 5));
    C->no_vessels_in_hand = k;
    makezero(k);
    C->state = JOBLESS;
    printf("\n Robot Chef %d has prepared %d vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking", C->i, C->no_vessels_in_hand);
}
void biryani_ready(Cook *C)
{
    int key;
    makezero(key);
    while (!key)
    {
        int i;
        makezero(i);
        while (i < no_table)
        {
            if (!C->no_vessels_in_hand)
                return;
            pthread_mutex_lock(&tables[i]->mutex);
            if (tables[i]->state == WAITING)
            {
                C->no_vessels_in_hand = C->no_vessels_in_hand - 1;
                makezero(key);
                tables[i]->no_slots_left = C->no_slots_in_vessel;
                tables[i]->state = SERVING;
                printf("\n Serving Container of Table %d is refilled by Robot Chef %d; Table %d resuming serving now", i, C->i, i);
            }
            pthread_mutex_unlock(&tables[i]->mutex);
            i = i + 1;
        }
    }
}
void *Table_thread(void *args)
{
    Table *T = (Table *)args;
    int key;
    makezero(key);
    while (!key)
    {
        makezero(key);
        wait_for_vessel(T);
        printf("\n Serving table %d entering Serving Phase", T->i);
        makezero(key);
        ready_to_serve_table(T);
    }
}
void wait_for_vessel(Table *T)
{
    int key;
    makezero(key);
    while (!key)
    {
        pthread_mutex_lock(&T->mutex);
        if (T->state == SERVING)
        {
            pthread_mutex_unlock(&T->mutex);
            return;
        }
        pthread_mutex_unlock(&T->mutex);
    }
}
void ready_to_serve_table(Table *T)
{
    int key;
    makezero(key);
    while (!key)
    {
        pthread_mutex_lock(&T->mutex);
        if (!T->no_slots_available)
        {
            if (!T->no_slots_left)
            {
                T->state = WAITING;
                pthread_mutex_unlock(&T->mutex);
                printf("\n Serving Container of Table %d is empty, waiting for refill", T->i);
                return;
            }
            else
            {
                T->no_slots_available = random_integer(1, 10);
                if (T->no_slots_available > T->no_slots_left)
                    T->no_slots_available = T->no_slots_left;
                T->no_slots_left = T->no_slots_left - T->no_slots_available;
                printf("\n Serving Table %d is ready to serve with %d slots", T->i, T->no_slots_available);
            }
        }
        pthread_mutex_unlock(&T->mutex);
    }
}
void *Student_thread(void *args)
{
    int key;
    Student *S = (Student *)args;
    makezero(key);
    sleep(S->arrival_time);
    printf("\n Student %d has arrived", S->i);
    S->state = WAIT4SLOT;
    makezero(key);
    printf("\n Student %d is waiting to be allocated a slot on the serving table", S->i);
    wait_for_slot(S);
    makezero(key);
    S->state = SERVED;
    return NULL;
}
void wait_for_slot(Student *S)
{
    int key;
    makezero(key);
    while (!key)
    {
        int i = random_integer(0, no_table - 1);
        while (i < no_table)
        {
            pthread_mutex_lock(&tables[i]->mutex);
            if (tables[i]->no_slots_available > 0)
            {
                tables[i]->no_slots_available = tables[i]->no_slots_available - 1;
                pthread_mutex_unlock(&tables[i]->mutex);
                printf("\n Student %d assigned a slot on the serving table %d and waiting to be served", S->i, i);
                S->state = INSLOT;
                student_in_slot(S, i);
                S->state = SERVED;
                return;
            }
            pthread_mutex_unlock(&tables[i]->mutex);
            i = i + random_integer(1, 2);
        }
    }
}
void student_in_slot(Student *S, int j)
{
    printf("\n Student %d on serving table %d has been served.", S->i, j);
}
int main()
{
    srand(time(0));
    int i, flag = 1;
    printf("\n Enter the number of :\n");
    printf("\tRobot cooks:");
    scanf("%d", &no_cook);
    printf("\tServing tables:");
    scanf("%d", &no_table);
    printf("\tStudents:");
    scanf("%d", &no_student);
    printf("\n Simulation begins");
    cooks = (Cook **)malloc((sizeof(Cook *)) * (no_cook));
    makezero(i);
    while (i < no_cook)
    {
        cooks[i] = (Cook *)malloc(sizeof(Cook));
        cooks[i]->state = JOBLESS;
        cooks[i]->i = i;
        makezero(cooks[i]->no_vessels_in_hand);
        cooks[i]->no_slots_in_vessel = random_integer(25, 50);
        pthread_mutex_init(&cooks[i]->mutex, NULL);
        i = i + 1;
    }
    tables = (Table **)malloc((sizeof(Table *)) * (no_table));
    makezero(i);
    while (i < no_table)
    {
        tables[i] = (Table *)malloc(sizeof(Table));
        tables[i]->i = i;
        tables[i]->state = WAITING;
        makezero(tables[i]->no_slots_available);
        makezero(tables[i]->no_slots_left);
        pthread_mutex_init(&tables[i]->mutex, NULL);
        i = i + 1;
    }
    students = (Student **)malloc((sizeof(Student *)) * (no_student));
    makezero(i);
    while (i < no_student)
    {
        students[i] = (Student *)malloc(sizeof(Student));
        students[i]->i = i;
        students[i]->state = NOT_ARRIVED;
        int k = (int)(no_student * 1.5);
        students[i]->arrival_time = random_integer(0, k);
        i = i + 1;
    }
    makezero(i);
    while (i < no_cook)
    {
        pthread_create(&(cooks[i]->tid), NULL, Cook_thread, (void *)cooks[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < no_student)
    {
        pthread_create(&(students[i]->tid), NULL, Student_thread, (void *)students[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < no_table)
    {
        pthread_create(&(tables[i]->tid), NULL, Table_thread, (void *)tables[i]);
        i = i + 1;
    }
    makezero(i);
    while (i < no_student)
    {
        pthread_join(students[i]->tid, NULL);
        i = i + 1;
    }
    makezero(i);
    while (i < no_cook)
    {
        pthread_mutex_destroy(&cooks[i]->mutex);
        free(cooks[i]);
        i = i + 1;
    }
    free(cooks);
    makezero(i);
    while (i < no_table)
    {
        pthread_mutex_destroy(&tables[i]->mutex);
        free(tables[i]);
        i = i + 1;
    }
    free(tables);
    makezero(i);
    while (i < no_student)
    {
        free(students[i]);
        i = i + 1;
    }
    free(students);
    printf("\n Simulation Over.");
}