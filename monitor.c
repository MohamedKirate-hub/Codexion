# include "codexion.h"

void *monitoring(void *args)
{
    t_sim   *sim;
    int     i;
    long    time_to_burnout;

    time_to_burnout = sim->args->time_to_burnout;
    sim = (t_sim *)args;
    while (sim->running)
    {
        i = 0;
        while (i < sim->args->number_of_coders)
        {
            if (current_time() - sim->coders[i].last_compile > time_to_burnout)
            {
                print_state(&sim->coders[i], "burned out");
                sim->running = 0;
            }
            i++;
        }
    }
    return NULL;
}

void start_monitor(t_sim *sim)
{
    t_monitor   monitor;
    int         status;

    sim->running = 1;
    // pthread_mutex_init(&monitor.monitor_mutex, NULL);
    status = pthread_create(&monitor.monitor, NULL, monitoring, &sim);
    if (status)
        return;
}