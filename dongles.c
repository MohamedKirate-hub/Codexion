/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/18 12:05:42 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int sorting_methode(char *scheduler)
{
    if (strcmp(scheduler, "fifo") == 0)
        return 2;
    return 3;
}

void setup_dongles(t_sim *sim, int capacity)
{
    int i;
    int j;
    t_heap *heap;

    i = 0;
    heap = create_heap(capacity);
    while (i < capacity)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;

        heap = create_heap(capacity);
        j = 0;

        while (j < capacity)
            push(heap, &sim->coders[j++], sorting_methode(sim->args->scheduler));
        sim->dongles[i].heap = heap;
        i++;
    }
}