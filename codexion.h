/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:15 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/17 12:22:08 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// heap struct
typedef struct s_heap_node
{
    int id;
    char *name;
    long release_time;
} t_heap_node;

typedef struct s_heap
{
    t_heap_node **data;
    int capacity;
    int count;
} t_heap;

// parse struct

typedef struct t_args {
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
    
    long start_time;
    pthread_mutex_t mutex;
} s_args;

// coder struct
typedef struct t_coder
{
    int id;
    s_args *args;
} s_coder;

typedef struct s_heap
{
    int *data;
    int capacity;
    int count;
} t_heap;

int ft_atoi(char *str);
s_args parser(int ac, char **av);
