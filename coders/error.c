/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:25:49 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/18 11:09:25 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_parser_messages	*get_parser_messages(void)
{
	static t_parser_messages	msg;

	msg.alloc_error = "Error: memory allocation failed\n";
	msg.invalid_argc = "Error: wrong number of arguments, expected 9\n";
	msg.invalid_arg = "Error: argument must contain only digits\n";
	msg.dongle_alloc_error = "Error: allocation failed for dongles\n";
	msg.coder_alloc_error = "Error: allocation failed for coders\n";
	msg.heap_push_error = "Error: failed to push to heap\n";
	msg.invalid_schedule = "Error: scheduler must be 'edf' or 'fifo'\n";
	msg.mutex_init_error = "Error: mutex initialization failed\n";
	msg.cond_init_error = "Error: condition variable initialization failed\n";
	msg.cond_wait_error = "Error: condition variable wait failed\n";
	msg.cond_broadcast_error = "Error: condition variable broadcast failed\n";
	msg.invalid = "ERROR: empty or invalid argument.\n";
	msg.invalid_number = "ERROR: argument must contain digits only.\n";
	msg.overflow_error = "ERROR: argument value is too large.\n";
	return (&msg);
}

int	print_error(char *msg, int return_val)
{
	fprintf(stderr, "%s", msg);
	return (return_val);
}
