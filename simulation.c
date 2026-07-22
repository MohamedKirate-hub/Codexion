# include "codexion.h"

int finishing(t_sim *sim)
{
    t_coder coder;
    int     finished;
    int     i;

    finished = 1;
    i = 0;
    while (i < sim->args->number_of_coders)
    {
        coder = sim->coders[i];
        if (coder.compile_count < sim->args->number_of_compiles_required)
            finished = 0;
        i++;
    }
    return (finished);
}

void simulation(t_sim *sim)
{
    t_coder     *coder;
    t_monitor   *monitor;
    int         i;
    int         status;
    int         thread_status;

    monitor->sim = sim;
    monitor->coder = coder;
    monitor->time_to_burnout = sim->args->time_to_burnout;

    thread_status = pthread_create(&monitor->monitor, NULL, monitoring, &monitor);
    if (!thread_status)
        return (1);
    while (!finishing(sim) && monitor->sim->running)
    {
        i = 0;
        while (i < sim->args->number_of_coders)
        {
            coder = &sim->coders[i];
            if (request_dongles(coder, sim))
            {
                if (take_dongle(coder, sim))
                    join_coder(coder);
                    pthread_join(monitor->monitor, NULL);
                    cleaner(coder, sim);
                }
            }
            i++;
        }
    }
}
