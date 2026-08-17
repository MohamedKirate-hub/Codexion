/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:22:00 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/17 16:59:11 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongles(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->dongles[i].heap->heap_mutex);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		free(sim->dongles[i].heap->data);
		free(sim->dongles[i].heap);
		i++;
	}
	free(sim->dongles);
}

void	free_things(t_sim *sim, int coder_setup)
{
	if (coder_setup)
		free(sim->coders);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->sim_mutex);
	pthread_mutex_destroy(&sim->args->mutex);
	free(sim->monitor);
}

void	clean_dongles(t_sim *sim, int fail_index)
{
	free_dongles(sim, fail_index);
	free_things(sim, 0);
}
