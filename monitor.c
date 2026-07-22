# include "codexion.h"

void *monitoring(void *args)
{
    long    spend_time;
    t_monitor *monitor;

    monitor = (t_monitor *)args;
    spend_time = current_time() - monitor->coder->last_compile;
    if (spend_time > monitor->time_to_burnout)
    {
        print_state(monitor->coder, "burned out");
        return (void *)1;
    }
    return (void *)0;
}