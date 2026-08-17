/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_race_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:58:32 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/17 15:59:33 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_sleeping(t_sim *sim)
{
	int	state;

	pthread_mutex_lock(&sim->sleep);
	state = sim->sleeping;
	pthread_mutex_unlock(&sim->sleep);
	return (state);
}

void	set_sleeping(t_sim *sim, int val)
{
	pthread_mutex_lock(&sim->sleep);
	sim->sleeping = val;
	pthread_mutex_unlock(&sim->sleep);
}