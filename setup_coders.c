/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:53:10 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/17 17:10:44 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	thread_error(t_sim *sim, int i)
{
	set_running(sim, 0);
	fprintf(stderr, "Failed to create thread %d\n", i);
	while (i > 0)
	{
		i--;
		pthread_join(sim->coders[i].thread, NULL);
	}
	destroy_dongles(sim, sim->args->number_of_coders);
	return (1);
}

int	setup_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		init_coder(sim, i);
		if (pthread_create(&sim->coders[i].thread, NULL,
				working, &sim->coders[i]))
			return (thread_error(sim, i));
		i++;
	}
	if (!start_monitor(sim))
	{
		i = 0;
		while (i < sim->args->number_of_coders)
			pthread_join(sim->coders[i++].thread, NULL);
		destroy_dongles(sim, i);
		return (1);
	}
	return (0);
}
