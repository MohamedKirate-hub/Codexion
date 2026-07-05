#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *args){
    int *number = args;
    printf("The number it pass is: %d", *number);
    return "Done";
}

int main()
{
    pthread_t thread1;
    void *thread_res;
    int res;

    int number = 101;
    res = pthread_create(&thread1, NULL, thread_function, &number);
    if (res != 0)
    {
        perror("Creating thread is failed");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, &thread_res);
    printf("\n%s\n", (char *)thread_res);
}