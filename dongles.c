/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/13 11:17:45 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int setup_dongles(t_sim *sim, int capacity)
{
    int i;
    t_heap *heap;

    i = 0;
    while (i < capacity)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;
        sim->dongles[i].releast_time = 0;
        heap = create_heap(capacity);
        sim->dongles[i].heap = heap;
        if (!sim->dongles[i].heap)
        {
            while (--i >= 0)
            {
                free(sim->dongles[i].heap);
                pthread_mutex_destroy(&sim->dongles[i].mutex);
            }
            free(sim->dongles);
            return (1);
        }
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
            return (1);
        
        if (pthread_cond_init(&sim->dongles[i].cond, NULL))
            return (1);
        i++;
    }
    return (0);
}
