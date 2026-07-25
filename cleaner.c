/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:22:00 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/25 11:58:17 by mkirate          ###   ########.fr       */
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
        j = 0;
        while (j < sim->dongles[i].heap->count)
            free(sim->dongles[i].heap->data[j++]);
        free(sim->dongles[i].heap);
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        i++;
    }
    free(sim->dongles);
}

