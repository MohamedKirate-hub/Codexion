/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:19 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/22 17:03:11 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int number_validator(char *str)
{
    if (*str == '+' || *str == '-')
        str++;

    if (!*str)
        return (0);

    while (*str)
    {
        if (*str >= '0' && *str <= '9')
            str++;
        else
            return (0);
    }
    return (1);
    
}

int string_validator(char *str)
{
    if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
        return (1);
    return (0);
}

t_args parser(char **av)
{
    t_args data;

    data.number_of_coders = ft_atoi(av[1]);
    data.time_to_burnout = ft_atoi(av[2]);
    data.time_to_compile = ft_atoi(av[3]);
    data.time_to_debug = ft_atoi(av[4]);
    data.time_to_refactor = ft_atoi(av[5]);
    data.number_of_compiles_required = ft_atoi(av[6]);
    data.dongle_cooldown = ft_atoi(av[7]);
    if (string_validator(av[8]))
        data.scheduler = av[8];

    return (data);
}
