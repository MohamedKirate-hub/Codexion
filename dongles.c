/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/09 19:41:41 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_dongle(t_dongle *dongle, int coder_id,
                 long coldown)
{
    pthread_mutex_lock(&dongle->mutex);
    if (coder_id == dongle->heap->data[0]->id
        && dongle->available && current_time() - dongle->releast_time >= coldown)
    {
        dongle->available = 0;
        pthread_mutex_unlock(&dongle->mutex);
        return (1);
    }
    pthread_mutex_unlock(&dongle->mutex);
    return (0);
}

// void reserver_dongle(t_dongle *first, t_dongle* second, t_coder *coder, int order)
// {
//     int checking_hand;

//     if (order == 0)
//         checking_hand = coder->right_taked;
//     else
//         checking_hand = coder->left_taked;
//     if (check_dongle(first, coder->id, coder->args->dongle_cooldown))
//         {
//             pthread_mutex_lock(&first->mutex);
//             coder->right = first;
//             coder->right_taked = 1;
//             pthread_mutex_unlock(&first->mutex);
//         }

//         if (checking_hand)
//         {
//             if (check_dongle(second, coder->id, coder->sim->args->dongle_cooldown))
//             {
//                 pthread_mutex_lock(&second->mutex);
//                 pop(first->heap, coder->sim->args->scheduler);
//                 pop(second->heap, coder->sim->args->scheduler);
//                 coder->left = second;
//                 coder->left_taked = 1;
//                 print_state(coder, "has take a dongle");
//                 print_state(coder, "has take a dongle");
//                 pthread_mutex_unlock(&second->mutex);
//             }
//             else
//             {
//                 pthread_mutex_lock(&first->mutex);
//                 first->available = 1;
//                 coder->right_taked = 0;
//                 coder->right = NULL;
//                 pthread_mutex_unlock(&first->mutex);
//             }
//         }
// }

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
        return (-1);
    if (coder->id % 2 == 0)
        reserver_dongle(&sim->dongles[left_id], &sim->dongles[right_id], coder, 0);
    else
        reserver_dongle(&sim->dongles[right_id], &sim->dongles[left_id], coder, 1);
    if (coder->right_taked == 1 && coder->left_taked == 1)
        return (1);
    else
    {
        pop(sim->dongles[left_id].heap, sim->args->scheduler);
        pop(sim->dongles[right_id].heap, sim->args->scheduler);
        return (0);
    }
}

// int take_dongle(t_coder *coder, t_sim *sim)
// {
//     int left_id;
//     int right_id;

//     left_id = coder->ids[0];
//     right_id = coder->ids[1];

//     if (push(sim->dongles[left_id].heap, coder, sim->args->scheduler))
//         return (-1);
//     if (push(sim->dongles[right_id].heap, coder, sim->args->scheduler))
//         return (-1);
//     if (coder->id % 2 == 0)
//     {
//         if (check_dongle(&sim->dongles[right_id], coder->id, sim->args->dongle_cooldown))
//         {
//             pthread_mutex_lock(&sim->dongles[right_id].mutex);
//             coder->right = &sim->dongles[right_id];
//             coder->right_taked = 1;
//             pthread_mutex_unlock(&sim->dongles[right_id].mutex);
//         }

//         if (coder->right_taked)
//         {
//             if (check_dongle(&sim->dongles[left_id], coder->id, sim->args->dongle_cooldown))
//             {
//                 pthread_mutex_lock(&sim->dongles[left_id].mutex);
//                 pop(sim->dongles[left_id].heap, sim->args->scheduler);
//                 pop(sim->dongles[right_id].heap, sim->args->scheduler);
//                 coder->left = &sim->dongles[left_id];
//                 coder->left_taked = 1;
//                 print_state(coder, "has take a dongle");
//                 print_state(coder, "has take a dongle");
//                 pthread_mutex_unlock(&sim->dongles[left_id].mutex);
//             }
//             else
//             {
//                 pthread_mutex_lock(&sim->dongles[right_id].mutex);
//                 sim->dongles[right_id].available = 1;
//                 coder->right_taked = 0;
//                 coder->right = NULL;
//                 pthread_mutex_unlock(&sim->dongles[right_id].mutex);
//             }
//         }
//     }
//     else
//     {
//         if (check_dongle(&sim->dongles[left_id], coder->id, sim->args->dongle_cooldown))
//         {
//             pthread_mutex_lock(&sim->dongles[left_id].mutex);
//             sim->dongles[left_id].available = 0;
//             pop(sim->dongles[left_id].heap, sim->args->scheduler);
//             coder->left = &sim->dongles[left_id];
//             coder->left_taked = 1;
//             pthread_mutex_unlock(&sim->dongles[left_id].mutex);
//         }

//         if (coder->left_taked)
//         {
//             if (check_dongle(&sim->dongles[right_id], coder->id, sim->args->dongle_cooldown))
//             {
//                 pthread_mutex_lock(&sim->dongles[right_id].mutex);
//                 sim->dongles[right_id].available = 0;
//                 pop(sim->dongles[right_id].heap, sim->args->scheduler);
//                 coder->right = &sim->dongles[right_id];
//                 coder->right_taked = 1;
//                 print_state(coder, "has take a dongle");
//                 print_state(coder, "has take a dongle");
//                 pthread_mutex_unlock(&sim->dongles[right_id].mutex);
//             }
//             else
//             {
//                 pthread_mutex_lock(&sim->dongles[left_id].mutex);
//                 sim->dongles[left_id].available = 1;
//                 coder->left_taked = 0;
//                 coder->left = NULL;
//                 pthread_mutex_unlock(&sim->dongles[left_id].mutex);
//             }
//         }
//     }
//     if (coder->right_taked == 1 && coder->left_taked == 1)
//         return (1);
//     else
//     {
//         pop(sim->dongles[left_id].heap, sim->args->scheduler);
//         pop(sim->dongles[right_id].heap, sim->args->scheduler);
//         return (0);
//     }
// }

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
        if (!sim->dongles[i].heap)
        {
            while (i > 0)
                free(sim->dongles[--i].heap);
            return;
        }
        i++;
    }
}
