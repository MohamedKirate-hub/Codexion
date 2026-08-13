/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:26:58 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/13 17:04:03 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void coder_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->compile);
    coder->compile_count++;
    coder->last_compile = current_time() - coder->args->start_time;
    pthread_mutex_unlock(&coder->sim->compile);
    print_state(coder, "is compiling");
    usleep(coder->sim->args->time_to_compile * 1000);
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
    int     s;

    coder = (t_coder *)args;

    pthread_mutex_lock(&coder->sim->sim_mutex);
    while (!coder->sim->begin)
        pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mutex);
    pthread_mutex_unlock(&coder->sim->sim_mutex);

    if (coder->id % 2 != 0)
        usleep((coder->args->time_to_compile + coder->args->dongle_cooldown) / 2 * 1000);

    while (get_running(coder->sim))
    {
        if (coder->compile_count >= coder->sim->args->number_of_compiles_required)
            return (NULL);

        if (!get_running(coder->sim))
            break;
        s = take_dongles(coder);
        if (s == -1)
            return (NULL);
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
        usleep(1000);
    }
    return (NULL);
}

int setup_coders(t_sim *sim)
{
    int         i;

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        init_coder(sim, i);
        if (pthread_create(&sim->coders[i].thread, NULL, working, &sim->coders[i]))
        {
            fprintf(stderr, "Failed to create thread %d\n", i);
            set_running(sim, 0);
            while (--i >= 0)
                pthread_join(sim->coders[i].thread, NULL);
            destroy_dongles(sim, sim->args->number_of_coders);
            return (0);
        }
        i++;
    }
    start_monitor(sim);
    return (0);
}
