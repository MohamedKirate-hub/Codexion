/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 12:30:34 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/13 11:36:19 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void setup_order(t_coder *coder, t_dongle **first, int *f_taked,
                 t_dongle **second, int *s_taked)
{
    if (coder->id % 2 == 0)
    {
        *first = coder->right;
        *f_taked = coder->right_taked;
        *second = coder->left;
        *s_taked = coder->left_taked;
    }
    else
    {
        *first = coder->left;
        *f_taked = coder->left_taked;
        *second = coder->right;
        *s_taked = coder->right_taked;
    }
}

int single_dongle(t_coder *coder, t_dongle *dongle)
{
    if (push(dongle->heap, coder, coder->args->scheduler))
            return (0);
    while (get_running(coder->sim))
        usleep(1000);
    return (0);
}

int take_first(t_dongle *dongle, int *taked, t_coder *coder)
{
    while (!request_dongle(dongle, coder))
    {
        if (!get_running(coder->sim))
        {
            pthread_mutex_unlock(&dongle->mutex);
            return (-1);
        }
        usleep(500);
    }
    pthread_mutex_lock(&dongle->mutex);
    dongle->available = 0;
    *taked = 1;
    pthread_mutex_unlock(&dongle->mutex);
    return (1);
}

int	take_second(t_dongle *first, int *f_taked, t_dongle *second, int *s_taked,
                t_coder *coder)
{
	if (!request_dongle(second, coder))
	{
		pthread_mutex_unlock(&second->mutex);
		release_dongle(first, coder->args->scheduler, f_taked);
		return (0);
	}
    pthread_mutex_lock(&second->mutex);
	second->available = 0;
    *s_taked = 1;
	pthread_mutex_unlock(&second->mutex);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
    int         f_taked;
    int         s_taked;

	setup_order(coder, &first, &f_taked, &second, &s_taked);
    coder->arrived_time = current_time();
    if (first == second)
        return (single_dongle(coder, first));
    if (push(first->heap, coder, coder->args->scheduler))
        return (0);
    if (push(second->heap, coder, coder->args->scheduler))
        return (0);
	while (get_running(coder->sim))
	{
		take_first(first, &f_taked, coder);
		if (take_second(first, &f_taked, second, &s_taked, coder))
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
