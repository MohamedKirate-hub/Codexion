#include "codexion.h"

void coder_compiling(t_coder *coder)
{
    coder->compile_count++;
    coder->last_compile = current_time();
    print_state(coder, "is compiling");
    usleep(coder->args->time_to_compile * 1000);
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

void *thread_function(void *args)
{
    t_coder *data;

    data = (t_coder *)args;
    coder_compiling(data);
    coder_debugging(data);
    coder_refactoring(data);

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
        state = pthread_create(&threads[i], NULL, thread_function, &coders[i]);
        if (state)
        {
            fprintf(stderr, "Error in creating thread,\n");
            return (1);
        }
        coders[i].id = i+1;
        coders[i].args = args;
        coders[i].thread = threads[i];
        coders[i].releast_time = current_time();
        coders[i].compile_count = 0;
        coders[i].last_compile =  0;
        i++;
    }
    return (0);
}

int join_coders(t_coder *coders, t_args *args)
{
    int i;
    int state;

    i = 0;
    while(i < args->number_of_coders)
    {
        state = pthread_join(coders[i].thread, NULL);
        if (state)
        {
            fprintf(stderr, "Error in joining thread,\n");
            return (1);
        }
        i++;
    }
    return (0);
}

int main(void)
{
    t_args args;
    t_sim sim;

    args.time_to_compile = 200;
    args.time_to_debug = 100;
    args.time_to_refactor = 200;
    args.time_to_burnout = 600;
    args.number_of_coders = 2;

    sim.coders = malloc(sizeof(t_coder) * args.number_of_coders);
    if (!sim.coders)
        return (1);

    sim.dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    if (!sim.dongles)
    {
        free(sim.coders);
        return (1);
    }

    if (setup_coders(sim.coders, &args))
    {
        free(sim.coders);
        free(sim.dongles);
        return (1);
    }

    if (join_coders(sim.coders, &args))
    {
        free(sim.coders);
        free(sim.dongles);
        return (1);
    }

    free(sim.coders);
    free(sim.dongles);

    return (0);
}