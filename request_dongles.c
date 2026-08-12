/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 08:18:24 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/12 12:29:53 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int get_top_heap(t_dongle *dongle)
{
    int coder_id;

    pthread_mutex_lock(&dongle->heap->heap_mutex);
    coder_id = dongle->heap->data[0]->id;
    pthread_mutex_unlock(&dongle->heap->heap_mutex);
    return (coder_id);
}

int check_dongle(t_dongle *dongle)
{
    int check;

    pthread_mutex_lock(&dongle->mutex);
    check = dongle->available;
    pthread_mutex_unlock(&dongle->mutex);
    return (check);
}

int taked_rest(t_dongle *dongle, t_coder *coder)
{
    long compared_value;

    pthread_mutex_lock(&coder->sim->compile);
    compared_value = dongle->releast_time;
    if (current_time() - compared_value >= coder->args->dongle_cooldown)
    {
        pthread_mutex_unlock(&coder->sim->compile);
        return (1);
    }
    pthread_mutex_unlock(&coder->sim->compile);
    return (0);
}

int request_dongle(t_dongle *dongle, t_coder *coder)
{
    if (get_top_heap(dongle) != coder->id)
        return (0);
    if (!check_dongle(dongle))
        return (0);
    if (!taked_rest(dongle, coder))
        return (0);
    return (1);
}