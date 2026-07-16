#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct s_time_data
{
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    long start_time;
    pthread_mutex_t mutex;
} t_time_data;

typedef struct s_data{
    int id;
    t_time_data *shared;
} t_data;

long current_time()
{
    struct timeval tv;
    long sec_tv;
    long usec_tv;

    gettimeofday(&tv, NULL);
    sec_tv = tv.tv_sec;
    usec_tv = tv.tv_usec;
    return (sec_tv*1000 + usec_tv/1000);
}

void print_status(t_data *data, char *msg)
{
    long elapsed;
    int id;

    pthread_mutex_lock(&data->shared->mutex);
    elapsed = current_time() - data->shared->start_time;
    id = data->id;
    printf("%ld %d %s\n", elapsed, id, msg);
    pthread_mutex_unlock(&data->shared->mutex);
}

void *worker(void *args)
{
    struct timeval start, end;
    t_data *data = (t_data *)args;

    print_status(data, "has taken a dongle");

    print_status(data, "is compiling");
    usleep(data->shared->time_to_compile * 1000);

    print_status(data, "is debugging");
    usleep(data->shared->time_to_debug * 1000);

    print_status(data, "is refactoring");
    usleep(data->shared->time_to_refactor * 1000);

    return NULL;

}

int main()
{
    pthread_t threads[2];
    t_data data[2];
    t_time_data shared_data;
    int i = 0;

    shared_data.time_to_burnout = 300;
    shared_data.time_to_compile = 90;
    shared_data.time_to_debug = 90;
    shared_data.time_to_refactor = 90;

    pthread_mutex_init(&shared_data.mutex, NULL);

    shared_data.start_time = current_time();
    while (i < 2)
    {
        data[i].id = i;
        data[i].shared = &shared_data;
        pthread_create(&threads[i], NULL, worker, &data[i]);
        i++;
    }
    i = 0;
    while (i < 2)
        pthread_join(threads[i++], NULL);
    pthread_mutex_destroy(&shared_data.mutex);
    return (0);
}