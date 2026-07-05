#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x = 2;

void* thread_function()
{
    int i = 0;
    while (i < 100000)
    {
        i++;
        x++;
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_function, NULL);
    pthread_create(&t2, NULL, thread_function, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("the value of x is %d", x);
    return 0;
}