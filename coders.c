#include "codexion.h"

void coder_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->compile);
    coder->compile_count++;
    coder->last_compile = current_time();
    coder->arrived_time = current_time();
    pthread_mutex_unlock(&coder->sim->compile);
    print_state(coder, "is compiling");
    usleep(coder->sim->args->time_to_compile * 1000);
}

void coder_debugging(t_coder* coder)
{
    print_state(coder, "is debugging");
    usleep(coder->args->time_to_debug * 1000);
}

int coder_refactoring(t_coder* coder)
{
    print_state(coder, "is refactoring");
    usleep(coder->args->time_to_refactor * 1000);

    if (push(coder->left->heap, coder, coder->args->scheduler))
        return (1);
    if (push(coder->right->heap, coder, coder->args->scheduler))
        return (1);
    return (0);
}

void *working(void *args)
{
    t_coder *coder;
    int     s;

    coder = (t_coder *)args;
    pthread_mutex_lock(&coder->sim->sim_mutex);
    while (!coder->sim->begin)
        pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mutex);
    pthread_mutex_unlock(&coder->sim->sim_mutex);
    while (get_running(coder->sim))
    {
        if (coder->compile_count >= coder->sim->args->number_of_compiles_required)
            return (NULL);

        if (!get_running(coder->sim))
            break;
        s = take_dongle(coder, coder->sim);
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
            if (coder_refactoring(coder))
            {
                set_running(coder->sim, 0);
                return (NULL);
            }
        }
        usleep(1000);
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
        sim->coders[i].left_taked = 0;
        sim->coders[i].right_taked = 0;
        sim->coders[i].ids = malloc(sizeof(int) * 2);
        if (!sim->coders[i].ids)
        {
            while (i > 0)
                free(sim->coders[--i].ids);
            fprintf(stderr, "Error: failed to allocate memory for coder IDs.\n");
            return (1);
        }
        sim->coders[i].ids[0] = i;
        sim->coders[i].ids[1] = (i + 1) % sim->args->number_of_coders;
        // fill_heap(&sim->coders[i]);
        if (pthread_create(&threads[i], NULL, working, &sim->coders[i]))
        {
            printf("Failed to create thread %d\n", i);
            while (--i >= 0)
            {
                set_running(sim, 0);
                pthread_join(threads[i], NULL);
            }
            return (0);
        }
        sim->coders[i].thread = threads[i];
        i++;
    }
    start_monitor(sim);
    return (0);
}
