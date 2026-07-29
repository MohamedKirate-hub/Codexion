/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:35:06 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/29 19:41:53 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int ft_atoi(char *str)
{
    int sign;
    long tmp;
    long res;
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
        tmp = res;
        res = res * 10 + (str[i] - 48);
        if (tmp > res)
        {
            write(2, "Error: integer overflow.\n", 26);
            exit(2);
        }
        i++;
    }
    return (int)(res * sign);
}
