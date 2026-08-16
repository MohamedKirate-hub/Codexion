/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 12:30:34 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/16 12:59:16 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	setup_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->id % 2 == 0)
	{
		*first = coder->right;
		*second = coder->left;
	}
	else
	{
		*first = coder->left;
		*second = coder->right;
	}
}

int	single_dongle(t_coder *coder, t_dongle *dongle)
{
	if (push(dongle->heap, coder, coder->args->scheduler))
		return (1);
	while (get_running(coder->sim))
		usleep(1000);
	return (0);
}

int	take_first(t_dongle *dongle, t_coder *coder)
{
	while (!request_dongle(dongle, coder))
	{
		if (!get_running(coder->sim))
			return (1);
		usleep(500);
	}
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	take_second(t_dongle *first, t_dongle *second,
				t_coder *coder)
{
	if (!request_dongle(second, coder))
	{
		release_dongle(first);
		return (0);
	}
	pthread_mutex_lock(&second->mutex);
	second->available = 0;
	pthread_mutex_unlock(&second->mutex);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	setup_order(coder, &first, &second);
	coder->arrived_time = current_time();
	if (first == second)
		return (single_dongle(coder, first));
	if (push(first->heap, coder, coder->args->scheduler))
		return (0);
	if (push(second->heap, coder, coder->args->scheduler))
		return (0);
	while (get_running(coder->sim))
	{
		take_first(first, coder);
		if (take_second(first, second, coder))
		{
			print_state(coder, "has taken a dongle");
			print_state(coder, "has taken a dongle");
			pop(first->heap, coder->args->scheduler);
			pop(second->heap, coder->args->scheduler);
			return (1);
		}
	}
	return (0);
}
