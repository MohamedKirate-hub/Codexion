/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:21:22 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/17 16:55:03 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	reach_goal(t_sim *sim)
{
	int	i;
	int	compile_count;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->compile);
		compile_count = sim->coders[i].compile_count;
		pthread_mutex_unlock(&sim->compile);
		if (compile_count < sim->args->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	setup_monitor(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mutex);
	set_running(sim, 1);
	sim->args->start_time = current_time();
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mutex);
}

int	monitor_routine(t_sim *sim, t_coder *coder)
{
	long	time_to_burnout;
	long	last_compile;
	long	spend_time;

	time_to_burnout = sim->args->time_to_burnout;
	pthread_mutex_lock(&sim->compile);
	last_compile = coder->last_compile;
	spend_time = (current_time() - sim->args->start_time) - last_compile;
	pthread_mutex_unlock(&sim->compile);
	if (spend_time >= time_to_burnout)
	{
		set_running(sim, 0);
		print_state(coder, "burned out");
		return (0);
	}
	return (1);
}

void	*monitoring(void *args)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)args;
	setup_monitor(sim);
	while (get_running(sim))
	{
		i = 0;
		while (i < sim->args->number_of_coders)
		{
			if (!monitor_routine(sim, &sim->coders[i++]))
				break ;
		}
		if (!get_running(sim))
			break ;
		if (reach_goal(sim))
		{
			set_running(sim, 0);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

int	start_monitor(t_sim *sim)
{
	sim->monitor = (t_monitor *)malloc(sizeof(t_monitor));
	if (!sim->monitor)
	{
		set_running(sim, 0);
		return (0);
	}
	if (pthread_create(&sim->monitor->monitor, NULL, monitoring, sim))
	{
		free(sim->monitor);
		set_running(sim, 0);
		return (0);
	}
	pthread_join(sim->monitor->monitor, NULL);
	return (1);
}
