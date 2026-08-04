/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:21:22 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/25 12:07:00 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    if (sim->created_coders == sim->args->number_of_coders)
        pthread_cond_broadcast(&sim->cond);
    pthread_mutex_unlock(&sim->monitor->monitor_mutex);
    while (get_running(sim))
    {
        i = 0;
        while (i < sim->args->number_of_coders)
        {
            if (current_time() - sim->coders[i].last_compile >= time_to_burnout
                && sim->coders[i].compile_count < sim->args->number_of_compiles_required)
            {
                set_running(sim, 0);
                print_burnout(&sim->coders[i], "burned out");
                break;
            }
            i++;
        }
        if (reach_goal(sim))
        {
            set_running(sim, 0);
            break;
        }
        // usleep(500);
    }
    return NULL;
}

void start_monitor(t_sim *sim)
{
    int         status;

    set_running(sim, 1);
    sim->monitor = (t_monitor *)malloc(sizeof(t_monitor));
    pthread_mutex_init(&sim->monitor->monitor_mutex, NULL);
    status = pthread_create(&sim->monitor->monitor, NULL, monitoring, sim);
    if (status)
        return;
    pthread_join(sim->monitor->monitor, NULL);
}