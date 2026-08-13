/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:22:00 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/13 11:19:15 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void free_dongles(t_sim *sim, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        free(sim->dongles[i].heap->data);
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_mutex_destroy(&sim->dongles[i].heap->heap_mutex);
        free(sim->dongles[i].heap);
        i++;
    }
    free(sim->dongles);
}

void free_things(t_sim *sim, int coder_setup)
{
    int i;

    i = 0;
    if (coder_setup)
        free(sim->coders);
    pthread_mutex_destroy(&sim->print_mutex);
    pthread_mutex_destroy(&sim->sim_mutex);
    pthread_mutex_destroy(&sim->args->mutex);
    pthread_mutex_destroy(&sim->monitor->monitor_mutex);
    free(sim->monitor);
}

void clean_coders(t_sim *sim, int fail_index)
{
    int i;

    i = 0;
    while (i < fail_index)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    free_dongles(sim, sim->args->number_of_coders);
    free_things(sim, 1);
}

void clean_dongles(t_sim *sim, int fail_index)
{
    free_dongles(sim, fail_index);
    free_things(sim, 0);
}
