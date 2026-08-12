/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:26:58 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/10 15:32:00 by mkirat           ###   ########.fr       */
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
    int         state;
    pthread_t   threads[sim->args->number_of_coders];

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        init_coder(sim, i);
        if (!sim->coders[i].ids)
        {
            while (--i >= 0)
                free(sim->coders[i].ids);
            fprintf(stderr, "Error: failed to allocate memory for coder IDs.\n");
            return (1);
        }
        sim->coders[i].ids[0] = i;
        sim->coders[i].ids[1] = (i + 1) % sim->args->number_of_coders;
        // fill_heap(&sim->coders[i]);
        if (pthread_create(&threads[i], NULL, working, &sim->coders[i]))
        {
            fprintf(stderr, "Failed to create thread %d\n", i);
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
