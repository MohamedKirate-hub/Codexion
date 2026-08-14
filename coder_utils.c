/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:30:06 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/14 13:11:44 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void coder_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->compile);
    coder->compile_count++;
    coder->last_compile = current_time();
    pthread_mutex_unlock(&coder->sim->compile);
    print_state(coder, "is compiling");
    usleep(coder->sim->args->time_to_compile * 1000);
}

void coder_debugging(t_coder* coder)
{
    print_state(coder, "is debugging");
    usleep(coder->args->time_to_debug * 1000);
}

void coder_refactoring(t_coder* coder)
{
    print_state(coder, "is refactoring");
    usleep(coder->args->time_to_refactor * 1000);
}