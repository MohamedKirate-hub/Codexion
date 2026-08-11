/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 12:39:08 by mkirat            #+#    #+#             */
/*   Updated: 2026/08/11 13:19:15 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"


t_coder *remove_coder(t_heap *heap, int coder_id, char *schedelure)
{
    t_coder *removed_coder;
    int i;

    i = 0;
    pthread_mutex_lock(&heap->heap_mutex);
    while (i < heap->count && heap->data[i]->id != coder_id)
        i++;
    
    if (i == heap->count)
    {
        pthread_mutex_unlock(&heap->heap_mutex);
        return (NULL);
    }

    removed_coder = heap->data[i];
    heap->count--;
    heap->data[i] = heap->data[heap->count];
    if (i < heap->count)
    {
        bubble_down(heap, i, schedelure);
        bubble_up(heap, i, schedelure);
    }
    pthread_mutex_unlock(&heap->heap_mutex);
    return (removed_coder);
}