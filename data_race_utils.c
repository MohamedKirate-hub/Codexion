/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_race_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 13:09:59 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/16 13:17:00 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_begin(t_sim *sim)
{
	int	val;

	pthread_mutex_lock(&sim->run);
	val = sim->begin;
	pthread_mutex_unlock(&sim->run);
	return (val);
}

void	set_begin(t_sim *sim, int val)
{
	pthread_mutex_lock(&sim->run);
	sim->begin = val;
	pthread_mutex_unlock(&sim->run);
}

void	set_zero(t_sim *sim)
{
	set_running(sim, 0);
	set_begin(sim, 0);
}
