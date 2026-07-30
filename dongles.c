/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/25 10:10:56 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_dongle(t_dongle *dongle, long coldown)
{
    int valid;

    pthread_mutex_lock(&dongle->mutex);
    if (dongle->available && current_time() - dongle->releast_time > coldown)
    {
        pthread_mutex_unlock(&dongle->mutex);
        return (1);
    }
    pthread_mutex_unlock(&dongle->mutex);
    return (0);
}

int take_dongle(t_coder *coder, t_sim *sim)
{
    int left_id;
    int right_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];

    if (coder->id % 2 == 0)
    {
        if (check_dongle(&sim->dongles[right_id], sim->args->dongle_cooldown))
        {
            pthread_mutex_lock(&sim->dongles[right_id].mutex);
            sim->dongles[right_id].available = 0;
            pop(sim->dongles[right_id].heap, sim->args->scheduler);
            coder->right = &sim->dongles[right_id];
            coder->right_taked = 1;
            pthread_mutex_unlock(&sim->dongles[right_id].mutex);
        }

        if (coder->right_taked)
        {
            if (check_dongle(&sim->dongles[left_id], sim->args->dongle_cooldown))
            {
                pthread_mutex_lock(&sim->dongles[left_id].mutex);
                sim->dongles[left_id].available = 0;
                pop(sim->dongles[left_id].heap, sim->args->scheduler);
                coder->left = &sim->dongles[left_id];
                coder->left_taked = 1;
                print_state(coder, "has take a dongle");
                print_state(coder, "has take a dongle");
                pthread_mutex_unlock(&sim->dongles[left_id].mutex);
            }
            else
            {
                sim->dongles[right_id].available = 1;
                coder->right_taked = 0;
            }
        }

    }
    else
    {
        if (check_dongle(&sim->dongles[left_id], sim->args->dongle_cooldown))
        {
            pthread_mutex_lock(&sim->dongles[left_id].mutex);
            sim->dongles[left_id].available = 0;
            pop(sim->dongles[left_id].heap, sim->args->scheduler);
            coder->left = &sim->dongles[left_id];
            coder->left_taked = 1;
            pthread_mutex_unlock(&sim->dongles[left_id].mutex);
        }

        if (coder->left_taked)
        {
            if (check_dongle(&sim->dongles[right_id], sim->args->dongle_cooldown))
            {
                pthread_mutex_lock(&sim->dongles[right_id].mutex);
                sim->dongles[right_id].available = 0;
                pop(sim->dongles[right_id].heap, sim->args->scheduler);
                coder->right = &sim->dongles[right_id];
                coder->right_taked = 1;
                print_state(coder, "has take a dongle");
                print_state(coder, "has take a dongle");
                pthread_mutex_unlock(&sim->dongles[right_id].mutex);
            }
            else
            {
                sim->dongles[left_id].available = 1;
                coder->left_taked = 0;
            }
        }
    }
    return (coder->right_taked == 1 && coder->left_taked == 1 ? 1 : 0);
}

void setup_dongles(t_sim *sim, int capacity)
{
    int i;
    t_heap *heap;

    i = 0;
    while (i < capacity)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;
        sim->dongles[i].releast_time = 0;

        heap = create_heap(capacity);
        sim->dongles[i].heap = heap;
        i++;
    }
}
