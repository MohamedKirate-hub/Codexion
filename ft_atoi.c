/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:35:06 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/31 15:01:55 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void is_digit(char c)
{
    if (c >= '0' || c <= '9')
        return (1);
    return (0);
}

void is_overflow(int digit, int res, int sign)
{
    long int_max = 0;
    long int_min = 0;
    if (sign == 1 && (res > INT_MAX / 10
		|| (res == INT_MAX / 10 && digit > 7)))
    {
        printf("ERROR: INT overflow.\n");
        exit(1);
    }
}

int ft_atoi(char *str)
{
    int sign;
    int digit;
    int res;
    int i;

    i = 0;
    res = 0;
    sign = 1;

    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }

    while  (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        digit = str[i] - 48;
        is_overflow(digit, res, sign);
        res = res * 10 + digit;
        i++;
    }
    return (int)(res * sign);
}
