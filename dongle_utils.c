/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:17:54 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/14 10:57:18 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void release_dongle(t_dongle *dongle, char* scheduler)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->available = 1;
    pthread_mutex_unlock(&dongle->mutex);
}

void cleaner(t_coder *coder, t_sim *sim)
{
    pthread_mutex_lock(&coder->left->mutex);
    coder->left->available = 1;
    coder->left->releast_time = current_time();
    pthread_mutex_unlock(&coder->left->mutex);

    pthread_mutex_lock(&coder->right->mutex);
    coder->right->available = 1;
    coder->right->releast_time = current_time();
    pthread_mutex_unlock(&coder->right->mutex);
}
