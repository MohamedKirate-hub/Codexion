#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

# define NUMB 2

typedef struct s_heap
{
    s_heap_node **data;
    int capacity;
    int count;
} t_heap;

typedef struct s_time_data
{
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    long start_time;
    pthread_mutex_t mutex;
} t_time_data;

typedef struct t_heap_node
{
    s_coder *coder;
    long arrivale_time;
    long deadline;
} s_heap_node;

typedef struct t_dongle
{
    pthread_mutex_t mutex;
    int available;
} s_dongle;

typedef struct t_coder{
    int id;
    long last_compile;

    s_dongle left;
    s_dongle right;
    int left_taked;
    int right_taked;

    char* state;
    t_time_data *shared;

    s_heap_node *data;
} s_coder;

int main()
{
    pthread_t threads[NUMB];
    s_dongle dongles[NUMB];
    s_coder coders[NUMB];
}