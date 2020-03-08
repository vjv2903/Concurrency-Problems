
#define _POSIX_C_SOURCE 199309L
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#define lli long long int

void swap(lli *a, lli *b)
{
    lli t = *a;
    *a = *b;
    *b = t;
}

lli partition(lli *arr, lli low, lli high)
{
    lli element_pivot = rand() % (high - low + 1) + low;
    swap(&arr[high], &arr[element_pivot]);
    lli pivot = arr[high], i = low - 1;
    for (lli j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int insertion_sort(lli *arr, lli low, lli high)
{
    lli key, j;
    if (high - low + 1 >= 5 || high - low + 1 <= 0)
        return 0;
    for (lli i = low + 1; i <= high; i = i + 1)
    {
        j = i - 1;
        key = arr[i];
        while (j >= low && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    return 1;
}

lli *shareMem(size_t size)
{
    key_t mem_key = IPC_PRIVATE;
    lli shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    return (lli *)shmat(shm_id, NULL, 0);
}

struct packet
{
    lli l, r, *arr;
};

void concurrent_process_quicksort(lli *arr, lli low, lli high)
{
    if (low > high)
        _exit(1);
    if (!insertion_sort(arr, low, high))
    {
        lli pi = partition(arr, low, high);
        signed pid1 = fork(), status;
        if (!pid1)
        {
            concurrent_process_quicksort(arr, low, pi - 1);
            _exit(1);
        }
        else
        {
            lli pid2 = fork();
            if (!pid2)
            {
                concurrent_process_quicksort(arr, pi + 1, high);
                _exit(1);
            }
            else
            {
                waitpid(pid1, &status, 0);
                waitpid(pid2, &status, 0);
            }
        }
    }
}

void *concurrent_threaded_quicksort(void *a)
{
    struct packet *args = (struct packet *)a;
    lli low = args->l, high = args->r, *arr = args->arr;
    if (insertion_sort(arr, low, high))
        return NULL;
    lli pi = partition(arr, low, high);
    pthread_t tid1, tid2;
    struct packet a1, a2;
    a1.r = pi - 1;
    a1.arr = arr;
    a1.l = low;
    pthread_create(&tid1, NULL, concurrent_threaded_quicksort, &a1);
    a2.l = pi + 1;
    a2.arr = arr;
    a2.r = high;
    pthread_create(&tid2, NULL, concurrent_threaded_quicksort, &a2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

void normal_quicksort(lli *arr, lli low, lli high)
{
    if (low < high && !insertion_sort(arr, low, high))
    {
        lli pi = partition(arr, low, high);
        normal_quicksort(arr, low, pi - 1);
        normal_quicksort(arr, pi + 1, high);
    }
}

int main()
{
    struct timespec ts;
    srand(time(0));
    lli n, a[10000];
    scanf("%lld", &n);
    for (lli i = 0; i < n; i = i + 1)
        scanf("%lld", &a[i]);
    long double tt1, tt2, tt3, tt4;
    printf("\n concurrent quicksort using processes...");
    lli *arr = shareMem(sizeof(lli) * (n + 1));
    for (lli i = 0; i < n; i = i + 1)
        *(arr + i) = a[i];
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tt1 = ts.tv_nsec / (1e9) + ts.tv_sec;
    concurrent_process_quicksort(arr, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tt2 = ts.tv_nsec / (1e9) + ts.tv_sec;
    for (lli i = 0; i < n; i = i + 1)
        printf("%lld ", *(arr + i));
    shmdt(arr);
    printf("Time: %Lf", tt2 - tt1);
    printf("\n concurrent quicksort using threads.....");
    pthread_t tid;
    struct packet p;
    p.l = 0;
    p.arr = a;
    p.r = n - 1;
    pthread_create(&tid, NULL, concurrent_threaded_quicksort, &p);
    pthread_join(tid, NULL);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tt3 = ts.tv_nsec / (1e9) + ts.tv_sec;
    for (lli i = 0; i < n; i = i + 1)
        printf("%lld ", *(p.arr + i));
    printf("Time: %Lf", (tt3 - tt2));
    printf("\n normal quicksort.......................");
    normal_quicksort(a, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tt4 = ts.tv_nsec / (1e9) + ts.tv_sec;
    for (lli i = 0; i < n; i = i + 1)
        printf("%lld ", a[i]);
    printf("Time: %Lf", tt4 - tt3);
    printf("\nNormal_quicksort ran");
    printf("\n\t --- %Lf times faster than concurrent processes", (tt2 - tt1) / (tt4 - tt3));
    printf("\n\t --- %Lf times faster than concurrent threads", (tt3 - tt2) / (tt4 - tt3));
}
