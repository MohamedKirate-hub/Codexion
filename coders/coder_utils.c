/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:30:06 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/18 11:20:25 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->compile);
	coder->compile_count++;
	coder->last_compile = current_time() - coder->args->start_time;
	pthread_mutex_unlock(&coder->sim->compile);
	print_state(coder, "is compiling");
	fn_usleep(coder->sim->args->time_to_compile, coder->sim);
}

void	coder_debugging(t_coder *coder)
{
	print_state(coder, "is debugging");
	fn_usleep(coder->args->time_to_debug, coder->sim);
}

void	coder_refactoring(t_coder *coder)
{
	print_state(coder, "is refactoring");
	fn_usleep(coder->args->time_to_refactor, coder->sim);
}
