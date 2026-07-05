#include "codexion.h"

void *routine(void *args)
{
    int index = *(int *)args;
    int res = index * index;
    return (void *)(long)res;
}

int main()
{
    pthread_t threads[3];
    int i = 0;
    while (i < 3)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, routine, index);
        i++;
    }
    i = 0;
    while (i < 3)
    {
        void *res;
        pthread_join(threads[i], &res);
        printf("index thread %d return value %d\n", i, (int)(long)res);
        i++;
    }
    
    
}