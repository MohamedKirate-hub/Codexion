/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/22 17:09:13 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_dongle(t_dongle *dongle, long coldown)
{
    int valid;

    pthread_mutex_lock(&dongle->mutex);

    valid = 0;
    if (dongle->available && current_time() - dongle->releast_time > coldown)
        valid = 1;
    pthread_mutex_unlock(&dongle->mutex);
    return valid;
}

void take_dongle(t_coder *coder, t_sim *sim)
{
    int left_id;
    int right_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];

    if (coder->id % 2 == 0)
    {
        if (check_dongle(&sim->dongles[right_id], sim->args->dongle_cooldown))
        {
            sim->dongles[right_id].available = 0;
            pop(sim->dongles[right_id].heap, sim->args->scheduler);
            coder->right = &sim->dongles[right_id];
            coder->right_taked = 1;
            print_state(coder, "has take a dongle");
        }
        else
        {
            if (coder->left_taked)
            {
                sim->dongles[left_id].available = 1;
                coder->left_taked = 0;
            }
        }

        if (coder->right_taked)
        {
            if (check_dongle(&sim->dongles[left_id], sim->args->dongle_cooldown))
            {
                sim->dongles[left_id].available = 0;
                pop(sim->dongles[left_id].heap, sim->args->scheduler);
                coder->left = &sim->dongles[left_id];
                coder->left_taked = 1;
                print_state(coder, "has take a dongle");
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
            sim->dongles[left_id].available = 0;
            pop(sim->dongles[left_id].heap, sim->args->scheduler);
            coder->left = &sim->dongles[left_id];
            coder->left_taked = 1;
            print_state(coder, "has take a dongle");
        }
        else
        {
            if (coder->right_taked)
            {
                sim->dongles[right_id].available = 1;
                coder->right_taked = 0;
            }
        }

        if (coder->left_taked)
        {
            if (check_dongle(&sim->dongles[right_id], sim->args->dongle_cooldown))
            {
                sim->dongles[right_id].available = 0;
                pop(sim->dongles[right_id].heap, sim->args->scheduler);
                coder->right = &sim->dongles[right_id];
                coder->right_taked = 1;
                print_state(coder, "has take a dongle");
            }
            else
            {
                sim->dongles[left_id].available = 1;
                coder->left_taked = 0;
            }
        }
    }
}

void setup_dongles(t_sim *sim, int capacity)
{
    int i;
    t_heap *heap;

    i = 0;
    heap = create_heap(capacity);
    while (i < capacity)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;

        heap = create_heap(capacity);

        sim->dongles[i].heap = heap;
        i++;
    }
}
