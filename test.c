#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct t_shared
{
    int counter;
    int step;
    pthread_mutex_t mutex;
} s_shared;

typedef struct t_args
{
    int id;
    s_shared *safety;
} s_args;


void *thread_function(void *args)
{
    s_args *data = (s_args *)args;
    int i = 0;
    while (data->safety->step != data->id)
        usleep(1000);
    pthread_mutex_lock(&data->safety->mutex);
    printf("index is %d %d\n", data->id, data ->safety->step);
    data ->safety->step++;
    pthread_mutex_unlock(&data->safety->mutex);
    while (i < 100000)
    {
        i++;
        pthread_mutex_lock(&data->safety->mutex);
        data->safety->counter++;
        pthread_mutex_unlock(&data->safety->mutex);
    }

    return NULL;
}

int main()
{
    pthread_t threads[5];
    s_args data[5];
    s_shared shared;
    shared.step = 0;
    shared.counter = 0;

    pthread_mutex_init(&shared.mutex, NULL);
    int i = 0;
    while (i < 5)
    {
        data[i].id = i;
        data[i].safety = &shared;
        pthread_create(&threads[i], NULL, thread_function, &data[i]);
        i++;
    }

    i = 0;
    while (i < 5)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&shared.mutex);
    printf("COunter value is %d\n", shared.counter);
    return 0;
}