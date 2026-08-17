/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:27:18 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/17 21:05:56 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	setup_args_and_sim(t_args *args, t_sim *sim, int ac, char **av)
{
	int	valid;

	if (ac != 9)
	{
		fprintf(stderr, "%s", get_parser_messages()->invalid_argc);
		return (1);
	}
	*args = parser(ac, av, &valid);
	if (!valid)
		return (1);
	if (pthread_mutex_init(&args->mutex, NULL))
		return (print_error(get_parser_messages()->mutex_init_error, 1));
	sim->args = args;
	if (init_sim(sim))
		return (1);
	return (0);
}

int	alloc_sim(t_sim *sim, t_args *args)
{
	sim->coders = malloc(sizeof(t_coder) * args->number_of_coders);
	if (!sim->coders)
	{
		destroy_partial(sim, 5);
		return (print_error(get_parser_messages()->coder_alloc_error, 1));
	}
	sim->dongles = malloc(sizeof(t_dongle) * args->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		destroy_partial(sim, 5);
		return (print_error(get_parser_messages()->dongle_alloc_error, 1));
	}
	set_running(sim, 0);
	sim->error = 1;
	return (0);
}

int	start_sim(t_sim *sim, t_args *args)
{
	int	i;

	if (setup_dongles(sim, args->number_of_coders))
		return (1);
	if (setup_coders(sim))
		return (1);
	i = 0;
	while (i < args->number_of_coders)
		pthread_join(sim->coders[i++].thread, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_args	args;
	t_sim	sim;

	if (setup_args_and_sim(&args, &sim, ac, av))
		return (1);
	if (alloc_sim(&sim, &args))
		return (1);
	if (start_sim(&sim, &args))
		return (1);
	free_dongles(&sim, args.number_of_coders);
	free_things(&sim, args.number_of_coders);
	return (0);
}
