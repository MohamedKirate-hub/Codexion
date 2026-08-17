/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:26:58 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/17 22:36:14 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	waiting(t_coder *coder)
{
	long	spend_time;

	pthread_mutex_lock(&coder->sim->sim_mutex);
	while (!get_running(coder->sim) && coder->sim->error)
	{
		if (pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mutex))
		{
			if (get_running(coder->sim))
				print_error(get_parser_messages()->cond_wait_error, 0);
			pthread_mutex_unlock(&coder->sim->sim_mutex);
			return (0);
		}
	}
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	spend_time = coder->args->time_to_compile + coder->args->dongle_cooldown;
	if (coder->id % 2 != 0)
		fn_usleep(spend_time / 2 * 1000, coder->sim);
	return (1);
}

int	working_cycle(t_coder *coder)
{
	coder_compiling(coder);
	cleaner(coder);
	if (!get_running(coder->sim))
		return (0);
	coder_debugging(coder);
	if (!get_running(coder->sim))
		return (0);
	coder_refactoring(coder);
	return (1);
}

void	*working(void *args)
{
	t_coder	*coder;
	int		s;

	coder = (t_coder *)args;
	if (!waiting(coder))
		return (NULL);
	while (get_running(coder->sim))
	{
		if (!get_running(coder->sim))
			break ;
		s = take_dongles(coder);
		if (s == -1)
			return (NULL);
		if (s)
		{
			if (!get_running(coder->sim))
				break ;
			if (!working_cycle(coder))
				break ;
		}
		usleep(1000);
	}
	return (NULL);
}
