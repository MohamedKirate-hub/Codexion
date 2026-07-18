/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 09:15:25 by mkirat            #+#    #+#             */
/*   Updated: 2026/07/18 11:11:18 by mkirate          ###   ########.fr       */
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

void print_state(t_coder *data, char *state)
{
    t_coder *coder;
    long spend_time;

    coder = (t_coder *)data;
    spend_time = current_time() - coder->releast_time;
    pthread_mutex_lock(&coder->args->mutex);
    printf("%ld %d %s\n", spend_time, coder->id, state);
    pthread_mutex_unlock(&coder->args->mutex);
}