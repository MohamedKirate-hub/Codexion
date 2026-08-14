/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:53:10 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/14 21:09:29 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	setup_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		init_coder(sim, i);
		if (pthread_create(&sim->coders[i].thread, NULL,
				working, &sim->coders[i]))
		{
			fprintf(stderr, "Failed to create thread %d\n", i);
			set_running(sim, 0);
			while (--i >= 0)
				pthread_join(sim->coders[i].thread, NULL);
			destroy_dongles(sim, sim->args->number_of_coders);
			return (0);
		}
		i++;
	}
	start_monitor(sim);
	return (0);
}
