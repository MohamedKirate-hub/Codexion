/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:27:13 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/16 21:14:33 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_overflow(int digit, long res)
{
	if (res > INT_MAX / 10 || (res == INT_MAX / 10 && digit > 7))
		return (print_error(get_parser_messages()->overflow_error, 1));
	return (0);
}

long	ft_atoi(char *str)
{
	int		digit;
	long	res;
	int		i;

	i = 0;
	res = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (print_error(get_parser_messages()->invalid, -1));
	while (str[i])
	{
		if (!is_digit(str[i]))
			return (print_error(get_parser_messages()->invalid_number, -1));
		digit = str[i] - 48;
		if (is_overflow(digit, res))
			return (-1);
		res = res * 10 + digit;
		i++;
	}
	return (res);
}
