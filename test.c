#include "codexion.h"

#define NUMBER 50
#define ITER 3

typedef struct t_safe{
    pthread_mutex_t mutex;
} s_safe;

typedef struct t_info
{
    int id;
    s_safe *safety;
} s_info;

void *routine(void *args)
{
    s_info *x = (s_info *)args;
    int i = 0;
    while (i < ITER)
    {
        // pthread_mutex_lock(&x->safety->mutex);
        printf("[thread %d] doing something\n", x->id);
        // pthread_mutex_unlock(&x->safety->mutex);
        usleep(rand() % 1000);
        i++;
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUMBER];
    s_info info[NUMBER];
    s_safe safety;
    int i = 0;

    pthread_mutex_init(&safety.mutex, NULL);
    while (i < NUMBER)
    {
        info[i].id = i;
        info[i].safety = &safety;
        pthread_create(&threads[i], NULL, routine, &info[i]);
        i++;
    }
    i = 0;
    while (i < NUMBER)
        pthread_join(threads[i++], NULL);

    pthread_mutex_destroy(&safety.mutex);
    return 0;
}
