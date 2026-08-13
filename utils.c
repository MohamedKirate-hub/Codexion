/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 09:15:25 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/13 11:39:18 by mkirat           ###   ########.fr       */
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

int print_state(t_coder *coder, char *state)
{
    long spend_time;

    pthread_mutex_lock(&coder->sim->print_mutex);
    spend_time = current_time() - coder->args->start_time;
    if (get_running(coder->sim) || !strcmp(state, "burned out"))
        printf("%ld %d %s\n", spend_time, coder->id, state);
    pthread_mutex_unlock(&coder->sim->print_mutex);
    return (0);
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
    pthread_mutex_lock(&coder->left->mutex);
    coder->left->available = 1;
    coder->left->releast_time = current_time();
    coder->left_taked = 0;
    pthread_mutex_unlock(&coder->left->mutex);

    pthread_mutex_lock(&coder->right->mutex);
    coder->right->available = 1;
    coder->right->releast_time = current_time();
    coder->right_taked = 0;
    pthread_mutex_unlock(&coder->right->mutex);
}