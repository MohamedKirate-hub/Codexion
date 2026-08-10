/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:22:00 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/29 19:29:30 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void free_dongles(t_sim *sim)
{
    int i;
    int j;

    i = 0;
    while (i < sim->args->number_of_coders)
    {
        free(sim->dongles[i].heap->data);
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_mutex_destroy(&sim->dongles[i].heap->heap_mutex);
        free(sim->dongles[i].heap);
        i++;
    }
    free(sim->dongles);
}

void free_things(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->args->number_of_coders)
        free(sim->coders[i++].ids);
    free(sim->coders);
    pthread_mutex_destroy(&sim->print_mutex);
    pthread_mutex_destroy(&sim->sim_mutex);
    pthread_mutex_destroy(&sim->args->mutex);
    pthread_mutex_destroy(&sim->monitor->monitor_mutex);
    free(sim->monitor);
}