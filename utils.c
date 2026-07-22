/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 09:15:25 by mkirat            #+#    #+#             */
/*   Updated: 2026/07/20 14:49:40 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long current_time()
{
    struct timeval tv;
    long sec_tv;
    long usec_tv;
    
    gettimeofday(&tv, NULL);
    sec_tv = tv.tv_sec;
    usec_tv = tv.tv_usec;
    return (sec_tv*1000 + usec_tv/1000);
}

void print_state(t_coder *data, char *state, pthread_mutex_t mutex)
{
    t_coder *coder;
    long spend_time;

    coder = (t_coder *)data;
    spend_time = current_time() - coder->releast_time;
    pthread_mutex_lock(&mutex);
    printf("%ld %d %s\n", spend_time, coder->id, state);
    pthread_mutex_unlock(&mutex);
}

void fn_usleep(long time)
{
    long start;

    start = current_time();
    while (current_time() - start)
    {
        usleep(time / 1000);
        // if there are burn out break the raise error;
    }
}
int sorting_methode(char *scheduler)
{
    if (strcmp(scheduler, "fifo") == 0)
        return 2;
    return 3;
}

void cleaner(t_coder *coder, t_sim *sim)
{
    int right_id;
    int left_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];

    sim->dongles[left_id].available = 1;
    sim->dongles[right_id].available = 1;
    coder->left_taked = 0;
    coder->right_taked = 0;
}

void fill_heap(t_coder *coder)
{
    int         left_id;
    int         right_id;

    left_id = coder->ids[0];
    right_id = coder->ids[1];
    push(coder->sim->dongles[left_id].heap, coder, coder->sim->args->scheduler);
    push(coder->sim->dongles[right_id].heap, coder, coder->sim->args->scheduler);
}