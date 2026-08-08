/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 09:15:25 by mkirat            #+#    #+#             */
/*   Updated: 2026/07/29 19:20:19 by mkirat           ###   ########.fr       */
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

int get_running(t_sim *sim)
{
    int state;

    pthread_mutex_lock(&sim->run);
    state = sim->running;
    pthread_mutex_unlock(&sim->run);
    return state;
}

void set_running(t_sim *sim, int val)
{
    int state;

    pthread_mutex_lock(&sim->run);
    sim->running = val;
    pthread_mutex_unlock(&sim->run);
}

void print_state(t_coder *coder, char *state)
{
    long spend_time;

    pthread_mutex_lock(&coder->sim->print_mutex);
    spend_time = current_time() - coder->args->start_time;
    if (get_running(coder->sim) || !strcmp(state, "burned out"))
        printf("%ld %d %s\n", spend_time, coder->id, state);
    pthread_mutex_unlock(&coder->sim->print_mutex);
}

void print_burnout(t_coder *coder, char *state)
{
    long spend_time;

    pthread_mutex_lock(&coder->sim->monitor->monitor_mutex);
    spend_time = current_time() - coder->args->start_time;
    printf("%ld %d %s\n", spend_time, coder->id, state);
    pthread_mutex_unlock(&coder->sim->monitor->monitor_mutex);
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

    pthread_mutex_lock(&sim->dongles[left_id].mutex);
    sim->dongles[left_id].available = 1;
    sim->dongles[left_id].releast_time = current_time();
    coder->left_taked = 0;
    pthread_mutex_unlock(&sim->dongles[left_id].mutex);

    pthread_mutex_lock(&sim->dongles[right_id].mutex);
    sim->dongles[right_id].available = 1;
    sim->dongles[right_id].releast_time = current_time();
    coder->right_taked = 0;
    pthread_mutex_unlock(&sim->dongles[right_id].mutex);
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