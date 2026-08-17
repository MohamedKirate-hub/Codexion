/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:19 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/16 13:04:06 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	string_validator(char *str)
{
	if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
		return (1);
	return (0);
}

int	validate_args(int ac, char **av, int *value)
{
	int	i;

	i = 0;
	while (++i < ac)
	{
		if (i <= 7)
		{
			if (ft_atoi(av[i]) == -1)
			{
				*value = 0;
				return (0);
			}
		}
		else if (i == 8)
		{
			if (!string_validator(av[i]))
			{
				*value = 0;
				return (print_error(get_parser_messages()->invalid_schedule,
						0));
			}
		}
	}
	*value = 1;
	return (1);
}

void	assign_args(t_args *data, char **av)
{
	data->number_of_coders = ft_atoi(av[1]);
	data->time_to_burnout = ft_atoi(av[2]);
	data->time_to_compile = ft_atoi(av[3]);
	data->time_to_debug = ft_atoi(av[4]);
	data->time_to_refactor = ft_atoi(av[5]);
	data->number_of_compiles_required = ft_atoi(av[6]);
	data->dongle_cooldown = ft_atoi(av[7]);
	data->scheduler = av[8];
}

t_args	parser(int ac, char **av, int *valid)
{
	t_args	data;

	if (!validate_args(ac, av, valid))
		return (data);
	assign_args(&data, av);
	return (data);
}
