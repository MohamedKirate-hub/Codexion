/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:15 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/18 11:59:12 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// parse struct

// coder struct

typedef struct s_dongle
{
    int             id;
    int             available;
    t_heap          *heap;
    pthread_mutex_t mutex;
} t_dongle;

typedef struct s_args {
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;

    pthread_mutex_t mutex;
    long start_time;
} t_args;

typedef struct s_coder{
    int         id;
    long        releast_time;
    long        last_compile;
    int         compile_count;
    pthread_t   thread;

    t_dongle    left;
    t_dongle    right;
    int         left_taked;
    int         right_taked;

    t_args      *args;
} t_coder;

typedef struct s_sim
{
    t_args *args;
    t_coder *coders;
    t_dongle *dongles;
} t_sim;

typedef struct s_heap
{
    t_coder **data;
    int capacity;
    int count;
} t_heap;

int ft_atoi(char *str);
t_args parser(int ac, char **av);

void push(t_heap *heap, t_coder *coder, int method);
t_coder *pop(t_heap *heap, int method);
t_heap *create_heap(int capacity);

long current_time();
void print_state(t_coder *data, char *state);
