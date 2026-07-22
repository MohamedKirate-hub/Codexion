#include "codexion.h"

void coder_compiling(t_coder *coder, pthread_mutex_t mutex)
{
    coder->compile_count++;
    coder->last_compile = current_time();
    print_state(coder, "is compiling", mutex);
    usleep(coder->args->time_to_compile * 1000);

    push(coder->left->heap, coder, coder->args->scheduler);
    push(coder->right->heap, coder, coder->args->scheduler);
}

void coder_debugging(t_coder* coder, pthread_mutex_t mutex)
{
    print_state(coder, "is debugging", mutex);
    usleep(coder->args->time_to_debug * 1000);
}

void coder_refactoring(t_coder* coder, pthread_mutex_t mutex)
{
    print_state(coder, "is refactoring", mutex);
    usleep(coder->args->time_to_refactor * 1000);
}

void *working(void *args)
{
    t_coder *coder;
    pthread_mutex_t mutex;

    coder = (t_coder *)args;
    pthread_mutex_init(&mutex, NULL);
    coder_compiling(coder, coder->left->mutex);
    coder_debugging(coder, mutex);
    coder_refactoring(coder, mutex);

    pthread_mutex_destroy(&mutex);
    return (NULL);
}

int setup_coders(t_coder *coders, t_args *args)
{
    int         i;
    int         state;
    pthread_t   threads[args->number_of_coders];
    args->start_time = current_time();

    i = 0;
    while (i < args->number_of_coders)
    {
        coders[i].id = i+1;
        coders[i].args = args;
        coders[i].thread = threads[i];
        coders[i].releast_time = current_time();
        coders[i].compile_count = 0;
        coders[i].last_compile =  0;
        coders[i].ids[0] = i;
        coders[i].ids[1] = (i + 1) % args->number_of_coders;

        // state = pthread_create(&threads[i], NULL, working, &coders[i]);
        // if (state)
        // {
        //     fprintf(stderr, "Error in creating thread,\n");
        //     return (1);
        // }
        i++;
    }
    return (0);
}

int join_coder(t_coder *coder)
{
    int state;

    state = pthread_join(coder->thread, NULL);
    if (state)
    {
        fprintf(stderr, "Error in joining thread,\n");
        return (1);
    }
    return (0);
}
