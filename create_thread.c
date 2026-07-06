#include "codexion.h"

int x = 0;
pthread_mutex_t mutex;

void *routine()
{
    int i = 0;
    while (i < 10000000)
    {
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
        i++;
    }
    return NULL;
}

int main()
{
    pthread_t threads[2];
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&threads[0], NULL, routine, NULL);
    pthread_create(&threads[1], NULL, routine, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("x is %d.\n", x);
    pthread_mutex_destroy(&mutex);
    return 0;
}