#include "codexion.h"

void coder_compiling(t_coder *coder)
{
    coder->compile_count++;
    coder->last_compile = current_time();

    print_state(coder, "is compiling");
    usleep(coder->sim->args->time_to_compile * 1000);

    coder->arrived_time = current_time();
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

    coder = (t_coder *)args;
    pthread_mutex_lock(&coder->sim->sim_mutex);
    while (!coder->sim->begin)
        pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mutex);
    pthread_mutex_unlock(&coder->sim->sim_mutex);
    printf("start coder id %d state: %d\n", coder->id, get_running(coder->sim));
    while (get_running(coder->sim))
    {
        if (coder->compile_count >= coder->sim->args->number_of_compiles_required)
            return (NULL);

        if (!get_running(coder->sim))
            break;

        int s = take_dongle(coder, coder->sim);
        printf("coder working id: %d\n", coder->id);
        if (s)
        {
            if (!get_running(coder->sim))
                break;

            coder_compiling(coder);
            cleaner(coder, coder->sim);
            if (!get_running(coder->sim))
                break;
            coder_debugging(coder);
            if (!get_running(coder->sim))
                break;
            coder_refactoring(coder);
        }
    }
    return (NULL);
}

int setup_coders(t_sim *sim)
{
    int         i;
    int         state;
    pthread_t   threads[sim->args->number_of_coders];

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        sim->coders[i].id = i+1;
        sim->coders[i].args = sim->args;
        sim->coders[i].sim = sim;
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile =  0;
        sim->coders[i].arrived_time = 0;
        sim->coders[i].ids = malloc(sizeof(int) * 2);
        if (!sim->coders[i].ids)
        {
            while (i > 0)
                free(sim->coders[--i].ids);
            fprintf(stderr, "Error: failed to allocate memory for coder IDs.\n");
            exit(1);   
        }
        sim->coders[i].ids[0] = i;
        sim->coders[i].ids[1] = (i + 1) % sim->args->number_of_coders;
        fill_heap(&sim->coders[i]);

        state = pthread_create(&threads[i], NULL, working, &sim->coders[i]);
        if (state)
        {
            fprintf(stderr, "Error in creating thread,\n");
            exit(1);
        }
        sim->coders[i].thread = threads[i];
        i++;
    }
    start_monitor(sim);
    return (0);
}
