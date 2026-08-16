/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:11:15 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/16 21:26:25 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define INT_MAX 2147483647

typedef struct s_heap		t_heap;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;
typedef struct s_args		t_args;
typedef struct s_sim		t_sim;
typedef struct s_monitor	t_monitor;

typedef struct s_dongle
{
	int				id;
	int				available;
	long			releast_time;
	t_heap			*heap;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	long		arrived_time;
	long		last_compile;
	long		compile_count;
	int			stop;

	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	t_sim		*sim;
	t_args		*args;
}	t_coder;

typedef struct s_heap
{
	t_coder			**data;
	pthread_mutex_t	heap_mutex;
	int				capacity;
	int				count;
}	t_heap;

typedef struct s_args
{
	long			number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			number_of_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	pthread_mutex_t	mutex;
	long			start_time;
}	t_args;

typedef struct s_sim
{
	t_args			*args;
	t_coder			*coders;
	t_dongle		*dongles;
	t_monitor		*monitor;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	run;
	pthread_mutex_t	compile;
	pthread_cond_t	cond;
	int				running;
}	t_sim;

typedef struct s_monitor
{
	t_coder			*coder;
	pthread_t		monitor;
	pthread_mutex_t	monitor_mutex;
	t_sim			*sim;
	long			time_to_burnout;
}	t_monitor;

typedef struct s_parser_messages
{
	char	*alloc_error;
	char	*invalid_argc;
	char	*invalid_arg;
	char	*dongle_alloc_error;
	char	*coder_alloc_error;
	char	*heap_push_error;
	char	*invalid_schedule;
	char	*mutex_init_error;
	char	*cond_init_error;
	char	*cond_wait_error;
	char	*cond_broadcast_error;
	char	*invalid;
	char	*invalid_number;
	char	*overflow_error;
}	t_parser_messages;

long				ft_atoi(char *str);
t_args				parser(int ac, char **av, int *valid);
int					push(t_heap *heap, t_coder *coder, char *scheduler);
void				bubble_up(t_heap *heap, int size, char *scheduler);
void				bubble_down(t_heap *heap, int size, char *scheduler);
t_coder				*pop(t_heap *heap, char *scheduler);
t_heap				*create_heap(int capacity);
void				swap(t_coder **a, t_coder **b);
long				current_time(void);
int					print_state(t_coder *data, char *state);
int					sorting_methode(char *scheduler);
void				cleaner(t_coder *coder);
int					setup_dongles(t_sim *sim, int capacity);
int					setup_coders(t_sim *sim);
void				start_monitor(t_sim *sim);
int					get_running(t_sim *sim);
void				set_running(t_sim *sim, int val);
void				set_zero(t_sim *sim);
long				get_compile_time(t_coder *coder);
long				get_arrived_time(t_coder *coder);
long				get_compile_count(t_coder *coder);
void				free_dongles(t_sim *sim, int count);
void				free_things(t_sim *sim, int coder_setup);
t_parser_messages	*get_parser_messages(void);
int					print_error(char *msg, int return_val);
int					init_sim(t_sim *sim);
void				init_coder(t_sim *sim, int i);
int					request_dongle(t_dongle *dongle, t_coder *coder);
int					take_dongles(t_coder *coder);
void				release_dongle(t_dongle *dongle);
int					compare_bubble_down(t_coder *parent, t_coder *child,
						int method);
int					compare_bubble_up(t_coder *parent, t_coder *child,
						int method);
void				destroy_partial(t_sim *sim, int step);
int					destroy_dongles(t_sim *sim, int fail_index);
void				fn_usleep(t_sim *sim, long time);
void				coder_compiling(t_coder *coder);
void				coder_debugging(t_coder *coder);
void				coder_refactoring(t_coder *coder);
void				*working(void *args);

#endif