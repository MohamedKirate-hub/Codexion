/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:15 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/02 11:39:28 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define INT_MAX 2147483647

typedef struct s_heap    t_heap;
typedef struct s_coder   t_coder;
typedef struct s_dongle  t_dongle;
typedef struct s_args    t_args;
typedef struct s_sim     t_sim;
typedef struct s_monitor    t_monitor;


typedef struct s_dongle
{
    int             id;
    int             available;
    long             releast_time;
    t_heap          *heap;
    pthread_mutex_t mutex;
} t_dongle;

typedef struct s_coder{
    int         id;
    long        arrived_time;
    long        last_compile;
    int         compile_count;
    int         stop;
    pthread_t   thread;

    t_dongle    *left;
    t_dongle    *right;
    int         left_taked;
    int         right_taked;
    int         *ids;

    t_sim       *sim;
    t_args      *args;
} t_coder;

typedef struct s_heap
{
    t_coder **data;
    pthread_mutex_t heap_mutex;
    int capacity;
    int count;
} t_heap;

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

typedef struct s_sim
{
    t_args          *args;
    t_coder         *coders;
    t_dongle        *dongles;
    t_monitor       *monitor;
    pthread_mutex_t sim_mutex;
    pthread_mutex_t print_mutex;
    pthread_cond_t  cond;
    int             running;
    int             created_coders;
} t_sim;

typedef struct s_monitor
{
    t_coder         *coder;
    pthread_t       monitor;
    pthread_mutex_t monitor_mutex;
    t_sim           *sim;
    long            time_to_burnout;
} t_monitor;

int ft_atoi(char *str);
t_args parser(char **av);

void push(t_heap *heap, t_coder *coder, char* scheduler);
t_coder *pop(t_heap *heap, char* scheduler);
t_heap *create_heap(int capacity);

// utils
long current_time();
void print_state(t_coder *data, char *state);
void fn_usleep(long time);
int sorting_methode(char *scheduler);
void cleaner(t_coder *coder, t_sim *sim);
void fill_heap(t_coder *coder);

// dongles
int take_dongle(t_coder *coder, t_sim *sim);
void setup_dongles(t_sim *sim, int capacity);

// coders
int setup_coders(t_sim *sim);

int request_dongles(t_coder *coder, t_sim *sim);

// monitor
void start_monitor(t_sim *sim);



