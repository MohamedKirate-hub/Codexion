/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utlits.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 09:15:25 by mkirat            #+#    #+#             */
/*   Updated: 2026/07/17 09:32:14 by mkirat           ###   ########.fr       */
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

void print_state(s_coder *data, char *state)
{
    s_coder *args;
    long spend_time;

    args = (s_coder *)data;
    spend_time = current_time() - args->args->start_time;
    pthread_mutex_lock(&args->args->mutex);
    printf("%ld %d %s\n", spend_time, args->id, state);
}