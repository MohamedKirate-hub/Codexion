/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:26:50 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/17 15:57:44 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_partial(t_sim *sim, int step)
{
	if (step >= 0)
		pthread_mutex_destroy(&sim->args->mutex);
	if (step >= 1)
		pthread_cond_destroy(&sim->cond);
	if (step >= 2)
		pthread_mutex_destroy(&sim->sim_mutex);
	if (step >= 3)
		pthread_mutex_destroy(&sim->print_mutex);
	if (step >= 4)
		pthread_mutex_destroy(&sim->run);
	if (step >= 5)
		pthread_mutex_destroy(&sim->compile);
	if (step >= 6)
		pthread_mutex_destroy(&sim->sleep);
}

int	init_compile_mutex(t_sim *sim)
{
	if (pthread_mutex_init(&sim->compile, NULL))
	{
		destroy_partial(sim, 4);
		return (print_error(get_parser_messages()->mutex_init_error, 0));
	}
	if (pthread_mutex_init(&sim->sleep, NULL))
	{
		destroy_partial(sim, 5);
		return (print_error(get_parser_messages()->mutex_init_error, 0));	
	}
	return (1);
}

int	init_sim(t_sim *sim)
{
	if (pthread_cond_init(&sim->cond, NULL))
	{
		destroy_partial(sim, 0);
		return (print_error(get_parser_messages()->cond_init_error, 1));
	}
	if (pthread_mutex_init(&sim->sim_mutex, NULL))
	{
		destroy_partial(sim, 1);
		return (print_error(get_parser_messages()->mutex_init_error, 1));
	}
	if (pthread_mutex_init(&sim->print_mutex, NULL))
	{
		destroy_partial(sim, 2);
		return (print_error(get_parser_messages()->mutex_init_error, 1));
	}
	if (pthread_mutex_init(&sim->run, NULL))
	{
		destroy_partial(sim, 3);
		return (print_error(get_parser_messages()->mutex_init_error, 1));
	}
	if (!init_compile_mutex(sim))
		return (1);
	return (0);
}

void	init_coder(t_sim *sim, int i)
{
	sim->coders[i].id = i + 1;
	sim->coders[i].args = sim->args;
	sim->coders[i].sim = sim;
	sim->coders[i].compile_count = 0;
	sim->coders[i].last_compile = 0;
	sim->coders[i].arrived_time = 0;
	sim->coders[i].left = &sim->dongles[i];
	sim->coders[i].right = &sim->dongles[(i + 1) % sim->args->number_of_coders];
}
