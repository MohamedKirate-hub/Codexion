/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_dongles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/17 17:17:01 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	destroy_dongles(t_sim *sim, int fail_index)
{
	int	i;

	i = 0;
	while (i < fail_index)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		if (sim->dongles[i].heap)
		{
			pthread_mutex_destroy(&sim->dongles[i].heap->heap_mutex);
			free(sim->dongles[i].heap->data);
			free(sim->dongles[i].heap);
		}
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	destroy_partial(sim, 5);
	return (1);
}

void	init_dongle(t_dongle *dongle, int index)
{
	dongle->id = index;
	dongle->available = 1;
	dongle->releast_time = 0;
}

int	init_dongle_stuff(t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL))
	{
		fprintf(stderr, "Failed to init dongle mutex\n");
		return (0);
	}
	if (pthread_cond_init(&dongle->cond, NULL))
	{
		pthread_mutex_destroy(&dongle->mutex);
		fprintf(stderr, "Failed to init dongle condition variable\n");
		return (0);
	}
	return (1);
}

int	setup_dongles(t_sim *sim, int capacity)
{
	int		i;
	t_heap	*heap;

	i = 0;
	while (i < capacity)
	{
		init_dongle(&sim->dongles[i], i);
		heap = create_heap(capacity);
		sim->dongles[i].heap = heap;
		if (!heap)
		{
			fprintf(stderr, "allocation of dongle heap failed.\n");
			destroy_dongles(sim, i);
			return (1);
		}
		if (!init_dongle_stuff(&sim->dongles[i]))
		{
			free(heap->data);
			free(heap);
			destroy_dongles(sim, i);
			return (1);
		}
		i++;
	}
	return (0);
}
