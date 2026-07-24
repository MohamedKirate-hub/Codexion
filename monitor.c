# include "codexion.h"

int reach_goal(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        if (sim->coders[i].compile_count < sim->args->number_of_compiles_required)
            return (0);
        i++;
    }
    return (1);
}

void *monitoring(void *args)
{
    t_sim   *sim;
    int     i;
    long    time_to_burnout;

    sim = (t_sim *)args;
    time_to_burnout = sim->args->time_to_burnout;
    pthread_mutex_lock(&sim->monitor->monitor_mutex);
    while (sim->running)
    {
        i = 0;
        while (i < sim->args->number_of_coders)
        {
            if (current_time() - sim->coders[i].last_compile > time_to_burnout)
            {
                print_state(&sim->coders[i], "burned out");
                sim->running = 0;
                break;
            }
            i++;
        }
        if (reach_goal(sim))
        {
            sim->running = 0;
            break;
        }
        usleep(1000);
    }
    pthread_mutex_unlock(&sim->monitor->monitor_mutex);
    return NULL;
}

void start_monitor(t_sim *sim)
{
    int         status;

    sim->running = 1;
    sim->monitor = (t_monitor *)malloc(sizeof(t_monitor));
    pthread_mutex_init(&sim->monitor->monitor_mutex, NULL);
    status = pthread_create(&sim->monitor->monitor, NULL, monitoring, sim);
    if (status)
        return;
}