/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:17:54 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/13 12:39:28 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void release_dongle(t_dongle *dongle, char* scheduler, int *taked)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->available = 1;
    *taked = 0;
    pthread_mutex_unlock(&dongle->mutex);
}
