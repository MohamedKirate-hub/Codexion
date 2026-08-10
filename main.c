/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:27:18 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/10 15:35:26 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int ac, char **av)
{
    t_args  args;
    t_sim   sim;
    int     status;
    int     valid;
    int     i;

    if (ac != 9)
    {
        fprintf(stderr,"%s", get_parser_messages()->invalid_argc);
        return (1);
    }

    args = parser(av, &valid);
    if (!valid)
        return (1);

    if (pthread_mutex_init(&args.mutex, NULL))
        return (print_error(get_parser_messages()->mutex_init_error, 1));
    sim.args = &args;

    if (init_sim(&sim))
        return (1);

    sim.coders = malloc(sizeof(t_coder ) * args.number_of_coders);
    if (!sim.coders)
        return (print_error(get_parser_messages()->coder_alloc_error, 1));
    sim.dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    if (!sim.dongles)
        return (print_error(get_parser_messages()->dongle_alloc_error, 1));
    
    sim.running = 0;
    sim.begin = 0;
    
    if (setup_dongles(&sim, args.number_of_coders))
        return (1);
    status = setup_coders(&sim);
    if (status)
        return (1);

    i = 0;
    while (i < args.number_of_coders)
        pthread_join(sim.coders[i++].thread, NULL);
    
    free_dongles(&sim, args.number_of_coders);
    free_things(&sim, args.number_of_coders);
}
