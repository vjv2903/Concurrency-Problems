
# QUESTION 1 : Concurrent Quicksort

## AIM

Implement a Concurrent version of Quicksort algorithm

## IMPLEMENTATION

- When the number of elements in the array for a process is less than 5, perform an insertion sort to sort the elements of that array.
- Otherwise, partition the array around the pivot such that all the elements with a value less than the pivot are positioned before it, while all the elements with a value greater than the pivot are positioned after. In case of equality, they can go on either side of the partition.
- Recurse for the low and high subarray.

### Concurrent quick sort using processes

Here, the recursion part is done using processes. Create a child process (using fork) that sorts the higher subarray by calling the same function again. The parent process would create another child process (using fork) that sorts the lower subarray parallely by calling the same function again. The parent process waits till both the children are done with their sorting process.

### Concurrent quick sort using threads

Here, the recursion part is done using threads. Create two threads that sorts two subarrays separately and join them using the pthread_join function.

### Normal quick sort

Here, the recursion part is done using functions. Recursively call the function to sort the upper and lower parts.

## COMPARISON OF THE PERFORMANCES

For comparing the performances, calculate the time taken for sorting the same array in the three methods and divide them to get a permformance ratio

To run the files run
> gcc -pthread q1.c  
> ./a.out

## SAMPLE RUN

10  
3 2 11 63 92 23 64 22 27 44

 concurrent quicksort using processes...2 3 11 22 23 27 44 63 64 92 Time: 0.000885  
 concurrent quicksort using threads.....2 3 11 22 23 27 44 63 64 92 Time: 0.000480  
 normal quicksort.......................2 3 11 22 23 27 44 63 64 92 Time: 0.000018  
Normal_quicksort ran  
         --- 48.811606 times faster than concurrent processes  
         --- 26.465689 times faster than concurrent threads
