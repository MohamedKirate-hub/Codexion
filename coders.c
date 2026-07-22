#include "codexion.h"

void coder_compiling(t_coder *coder)
{
    coder->compile_count++;
    coder->last_compile = current_time();

    print_state(coder, "is compiling");
    usleep(coder->args->time_to_compile * 1000);

    push(coder->left->heap, coder, coder->args->scheduler);
    push(coder->right->heap, coder, coder->args->scheduler);
}

void coder_debugging(t_coder* coder)
{
    print_state(coder, "is debugging");
    usleep(coder->args->time_to_debug * 1000);
}

void coder_refactoring(t_coder* coder)
{
    print_state(coder, "is refactoring");
    usleep(coder->args->time_to_refactor * 1000);
}

void *working(void *args)
{
    t_coder *coder;
    pthread_mutex_t mutex;

    coder = (t_coder *)args;
    pthread_mutex_init(&mutex, NULL);
     if (request_dongles(coder, coder->sim))
    {
        take_dongle(coder, coder->sim);
        coder_compiling(coder);
        coder_debugging(coder);
        coder_refactoring(coder);
        cleaner(coder, coder->sim);
    }

    pthread_mutex_destroy(&mutex);
    return (NULL);
}

int setup_coders(t_sim *sim)
{
    int         i;
    int         state;
    pthread_t   threads[sim->args->number_of_coders];
    sim->args->start_time = current_time();

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        sim->coders[i].id = i+1;
        sim->coders[i].args = sim->args;
        sim->coders[i].sim = sim;
        sim->coders[i].thread = threads[i];
        sim->coders[i].releast_time = current_time();
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile =  0;
        sim->coders[i].ids[0] = i;
        sim->coders[i].ids[1] = (i + 1) % sim->args->number_of_coders;
        fill_heap(&sim->coders[i]);

        state = pthread_create(&threads[i], NULL, working, &sim->coders[i]);
        if (state)
        {
            fprintf(stderr, "Error in creating thread,\n");
            return (1);
        }
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
