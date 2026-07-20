/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 12:05:32 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/20 14:59:35 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int sorting_methode(char *scheduler)
{
    if (strcmp(scheduler, "fifo") == 0)
        return 2;
    return 3;
}

void take_dongles(t_sim *sim)
{
    int i;
    int idx;

    i = 0;
    pthread_mutex_lock(&sim->sim_mutex);
    while (i < sim->args->number_of_coders)
    {
        if (sim->coders[i].id % 2 && !sim->coders[i].left_taked)
        {
            idx = sim->coders[i].id % sim->args->number_of_coders;
            if (sim->dongles[idx].available)
            {
                sim->dongles[idx].available = 0;
                sim->coders[i].right = sim->dongles[idx];
                print_state(&sim->coders[i], "has take a dongle");
            }
            else
            {
                if (sim->dongles[idx].available)
                    sim->dongles[idx].available = 0;
            }

            if (sim->dongles[idx - 1].available)
            {
                sim->dongles[idx - 1].available = 0;
                sim->coders[i].left = sim->dongles[idx - 1];
                print_state(&sim->coders[i], "has take a dongle");
            }
            else
            {
                if (sim->dongles[idx].available)
                    sim->dongles[idx].available = 0;
            }
        }
        else
        {
            idx = sim->coders[i].id % sim->args->number_of_coders;
            if (sim->dongles[idx - 1].available)
            {
                sim->dongles[idx - 1].available = 0;
                sim->coders[i].left = sim->dongles[idx - 1];
                print_state(&sim->coders[i], "has take a dongle");
            }
            else
            {
                if (sim->dongles[idx].available)
                    sim->dongles[idx].available = 0;
            }

            if (sim->dongles[idx].available)
            {
                sim->dongles[idx].available = 0;
                sim->coders[i].right = sim->dongles[idx];
                print_state(&sim->coders[i], "has take a dongle");
            }
            else
            {
                if (sim->dongles[idx - 1].available)
                    sim->dongles[idx - 1].available = 0;
            }
        }
        i++;
    }
    pthread_mutex_unlock(&sim->sim_mutex);
}

int check_dongle(t_dongle dongle, long compile_time, long coldown)
{
    if (dongle.available && current_time() - compile_time > coldown)
        return 0;
    return 1;
}

void setup_dongles(t_sim *sim, int capacity)
{
    int i;
    int j;
    t_heap *heap;

    i = 0;
    heap = create_heap(capacity);
    while (i < capacity)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;

        heap = create_heap(capacity);
        j = 0;

        while (j < capacity)
            push(heap, &sim->coders[j++], sorting_methode(sim->args->scheduler));
        sim->dongles[i].heap = heap;
        i++;
    }
}
