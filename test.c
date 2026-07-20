#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

# include "codexion.h"

int main()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    long start = tv.tv_sec*1000 + tv.tv_usec/1000;

    printf('hello');
    us
    usleep_function(10 * 1000);
}