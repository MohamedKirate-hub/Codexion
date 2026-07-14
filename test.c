#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

# define N 3

typedef struct t_shared
{
    pthread_mutex_t dongles[N];
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
} s_shared;

typedef struct t_coder
{
    int id;
    s_shared *shared;
} t_coder;

void *coder(void *args)
{
    
}