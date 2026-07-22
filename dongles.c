/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/20 14:59:35 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_dongle(t_dongle *dongle, long compile_time, long coldown)
{
    if (dongle.available && current_time() - compile_time > coldown)
        return 1;
    return 0;
}

void take_dongle(t_coder *coder, t_sim *sim)
{
    int left_id;
    int right_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];

    pthread_mutex_lock(&sim->sim_mutex);
    if (coder->id % 2 == 0)
    {
        if (check_dongle(&sim->dongles[right_id],
            coder->last_compile, sim->args->dongle_cooldown))
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
            if (check_dongle(&sim->dongles[left_id],
                coder->last_compile, sim->args->dongle_cooldown))
            {
                sim->dongles[left_id].available = 0;
                pop(sim->dongles[left_id].heap, sim->args->scheduler);
                coder->left = &sim->dongles[left_id];
                coder->left_taked = 1;
                print_state(coder, "has take a dongle");
            }
            else
            {
                sim->dongles[left_id].available = 1;
                coder->left_taked = 0;
            }
        }

    }
    else
    {
        if (check_dongle(&sim->dongles[left_id],
            coder->last_compile, sim->args->dongle_cooldown))
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
            if (check_dongle(&sim->dongles[right_id],
                coder->last_compile, sim->args->dongle_cooldown))
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
    pthread_mutex_unlock(&sim->sim_mutex);
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
