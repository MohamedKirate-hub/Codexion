/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:21:22 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/08 12:42:22 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int reach_goal(t_sim *sim)
{
    int i;
    int compile_count;

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        pthread_mutex_lock(&sim->compile);
        compile_count = sim->coders[i].compile_count;
        pthread_mutex_unlock(&sim->compile);
        if (compile_count < sim->args->number_of_compiles_required)
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
    long    last_compile;
    int     compile_count;

    sim = (t_sim *)args;
    time_to_burnout = sim->args->time_to_burnout;
    
    pthread_mutex_lock(&sim->sim_mutex);
    sim->begin = 1;
    set_running(sim, 1);
    sim->args->start_time = current_time();
    pthread_cond_broadcast(&sim->cond);
    pthread_mutex_unlock(&sim->sim_mutex);

    while (get_running(sim))
    {
        i = 0;
        usleep(1000);
        while (i < sim->args->number_of_coders)
        {
            pthread_mutex_lock(&sim->compile);
            last_compile = sim->coders[i].last_compile;
            compile_count = sim->coders[i].compile_count;
            pthread_mutex_unlock(&sim->compile);

            if ((current_time() - sim->args->start_time) - last_compile >= time_to_burnout
                && compile_count < sim->args->number_of_compiles_required)
            {
                print_state(&sim->coders[i], "burned out");
                set_running(sim, 0);
                break;
            }
            i++;
        }
        if (reach_goal(sim))
        {
            set_running(sim, 0);
            sim->begin = 0;
            break;
        }
    }
    return NULL;
}

void start_monitor(t_sim *sim)
{
    int         status;

    // set_running(sim, 0);
    sim->monitor = (t_monitor *)malloc(sizeof(t_monitor));
    pthread_mutex_init(&sim->monitor->monitor_mutex, NULL);
    status = pthread_create(&sim->monitor->monitor, NULL, monitoring, sim);
    if (status)
        return;
    pthread_join(sim->monitor->monitor, NULL);
}