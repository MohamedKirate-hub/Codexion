/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_race.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 11:18:02 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/14 16:51:32 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_running(t_sim *sim)
{
	int	state;

	pthread_mutex_lock(&sim->run);
	state = sim->running;
	pthread_mutex_unlock(&sim->run);
	return (state);
}

void	set_running(t_sim *sim, int val)
{
	int	state;

	pthread_mutex_lock(&sim->run);
	sim->running = val;
	pthread_mutex_unlock(&sim->run);
}

int	get_compile_count(t_coder *coder)
{
	int	compile_count;

	pthread_mutex_lock(&coder->sim->compile);
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->sim->compile);
	return (compile_count);
}

long	get_compile_time(t_coder *coder)
{
	long	compile_time;

	pthread_mutex_lock(&coder->sim->compile);
	compile_time = coder->last_compile;
	pthread_mutex_unlock(&coder->sim->compile);
	return (compile_time);
}

long	get_arrived_time(t_coder *coder)
{
	long	arrived_time;

	pthread_mutex_lock(&coder->sim->compile);
	arrived_time = coder->arrived_time;
	pthread_mutex_unlock(&coder->sim->compile);
	return (arrived_time);
}
