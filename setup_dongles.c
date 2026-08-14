/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/13 16:57:31 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int destroy_dongles(t_sim *sim, int fail_index)
{
    int i;

    i = 0;
    while (i < fail_index)
    {
        free(sim->dongles[i].heap);
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_cond_destroy(&sim->dongles[i].cond);
        i++;
    }
    free(sim->dongles);
    destroy_partial(sim, 5);
    return (1);
}

void init_dongle(t_dongle *dongle, int index)
{
    dongle->id = index;
    dongle->available = 1;
    dongle->releast_time = 0;
}

int setup_dongles(t_sim *sim, int capacity)
{
    int i;
    t_heap *heap;

    i = 0;
    while (i < capacity)
    {
        init_dongle(&sim->dongles[i], i);
        heap = create_heap(capacity);
        sim->dongles[i].heap = heap;
        if (!heap)
            return (destroy_dongles(sim, i));
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
        {
            free(heap);
            return (destroy_dongles(sim, i));
        }        
        if (pthread_cond_init(&sim->dongles[i].cond, NULL))
        {
            free(heap);
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            return (destroy_dongles(sim, i));
        }
        i++;
    }
    return (0);
}
