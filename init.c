/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:26:50 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/12 13:24:21 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_sim(t_sim *sim)
{
    if (pthread_cond_init(&sim->cond, NULL))
        return (print_error(get_parser_messages()->cond_init_error, 1));

    if (pthread_mutex_init(&sim->sim_mutex, NULL))
        return (print_error(get_parser_messages()->mutex_init_error, 1));

    if (pthread_mutex_init(&sim->print_mutex, NULL))
        return (print_error(get_parser_messages()->mutex_init_error, 1));
    
    if (pthread_mutex_init(&sim->run, NULL))
        return (print_error(get_parser_messages()->mutex_init_error, 1));

    if (pthread_mutex_init(&sim->compile, NULL))
        return (print_error(get_parser_messages()->mutex_init_error, 1));
    return (0);
}

void init_coder(t_sim *sim, int i)
{
    sim->coders[i].id = i+1;
    sim->coders[i].args = sim->args;
    sim->coders[i].sim = sim;
    
    sim->coders[i].compile_count = 0;
    sim->coders[i].last_compile =  0;
    sim->coders[i].arrived_time = 0;
    sim->coders[i].left_taked = 0;
    sim->coders[i].right_taked = 0;
    sim->coders[i].ids = malloc(sizeof(int) * 2);
    sim->coders[i].left = &sim->dongles[i];
    sim->coders[i].right = &sim->dongles[(i + 1) % sim->args->number_of_coders];
}
