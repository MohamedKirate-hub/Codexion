/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/11 13:20:18 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_dongle(t_dongle *dongle, int coder_id,
                 long coldown)
{
    pthread_mutex_lock(&dongle->mutex);
    pthread_mutex_lock(&dongle->heap->heap_mutex);
    if (coder_id == dongle->heap->data[0]->id
        && dongle->available && current_time() - dongle->releast_time >= coldown)
    {
        dongle->available = 0;
        pthread_mutex_unlock(&dongle->heap->heap_mutex);
        pthread_mutex_unlock(&dongle->mutex);
        return (1);
    }
    pthread_mutex_unlock(&dongle->heap->heap_mutex);
    pthread_mutex_unlock(&dongle->mutex);
    return (0);
}

void reserver_dongle(t_dongle *first, t_dongle *second, t_coder *coder, int order)
{
    int checking_hand;

    if (check_dongle(first, coder->id, coder->sim->args->dongle_cooldown))
    {
        pthread_mutex_lock(&first->mutex);
        if (order == 0)
        {
            coder->left = first;
            coder->left_taked = 1;
        }
        else
        {
            coder->right = first;
            coder->right_taked = 1;
        }
        pthread_mutex_unlock(&first->mutex);
    }

    if (order == 0)
        checking_hand = coder->left_taked;
    else
        checking_hand = coder->right_taked;

    if (checking_hand)
    {
        if (check_dongle(second, coder->id, coder->sim->args->dongle_cooldown))
        {
            pthread_mutex_lock(&second->mutex);
            // remove_coder(first->heap, coder->id, coder->sim->args->scheduler);
            // remove_coder(second->heap, coder->id, coder->sim->args->scheduler);
            pop(first->heap, coder->sim->args->scheduler);
            pop(second->heap, coder->sim->args->scheduler);
            if (order == 0)
            {
                coder->right = second;
                coder->right_taked = 1;
            }
            else
            {
                coder->left = second;
                coder->left_taked = 1;
            }
            print_state(coder, "has take a dongle");
            print_state(coder, "has take a dongle");
            pthread_mutex_unlock(&second->mutex);
        }
        else
        {
            pthread_mutex_lock(&first->mutex);
            first->available = 1;
            if (order == 0)
                coder->left_taked = 0;
            else
                coder->right_taked = 0;
            remove_coder(first->heap, coder->id, coder->sim->args->scheduler);
            pthread_mutex_unlock(&first->mutex);
        }
    }
}


int take_dongle(t_coder *coder, t_sim *sim)
{
    int left_id;
    int right_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];

    if (push(sim->dongles[left_id].heap, coder, sim->args->scheduler))
        return (-1);
    if (push(sim->dongles[right_id].heap, coder, sim->args->scheduler))
    {
        remove_coder(sim->dongles[left_id].heap, coder->id, coder->sim->args->scheduler);
        return (-1);
    }
    if (coder->id % 2 == 0)
        reserver_dongle(&sim->dongles[left_id], &sim->dongles[right_id], coder, 0);
    else
        reserver_dongle(&sim->dongles[right_id], &sim->dongles[left_id], coder, 1);
    if (coder->right_taked == 1 && coder->left_taked == 1)
        return (1);
    else
    {
        remove_coder(sim->dongles[left_id].heap, coder->id, coder->sim->args->scheduler);
        remove_coder(sim->dongles[right_id].heap, coder->id, coder->sim->args->scheduler);
        return (0);
    }
}

int setup_dongles(t_sim *sim, int capacity)
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
        if (!sim->dongles[i].heap)
        {
            while (--i >= 0)
            {
                free(sim->dongles[i].heap);
                pthread_mutex_destroy(&sim->dongles[i].mutex);
            }
            free(sim->dongles);
            return (1);
        }
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
            return (1);
        i++;
    }
    return (0);
}