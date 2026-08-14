/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:39:08 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/14 17:16:59 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	compare_bubble_up(t_coder *parent, t_coder *child, int method)
{
	long	parent_val;
	long	child_val;
	int		parent_compile;
	int		child_compile;

	parent_compile = get_compile_count(parent);
	child_compile = get_compile_count(child);
	if (method == 2)
	{
		parent_val = get_arrived_time(parent);
		child_val = get_arrived_time(child);
	}
	else
	{
		parent_val = get_compile_time(parent);
		child_val = get_compile_time(child);
	}
	if (parent_val > child_val)
		return (1);
	if (parent_compile > child_compile)
		return (1);
	return (0);
}

int	compare_bubble_down(t_coder *parent, t_coder *child, int method)
{
	long	parent_val;
	long	child_val;

	if (method == 2)
	{
		parent_val = get_arrived_time(parent);
		child_val = get_arrived_time(child);
	}
	else
	{
		parent_val = get_compile_time(parent);
		child_val = get_compile_time(child);
	}
	if (parent_val > child_val)
		return (1);
	return (0);
}
