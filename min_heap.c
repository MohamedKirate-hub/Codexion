/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:30:08 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/25 10:59:08 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void swap(t_coder **a, t_coder **b)
{
    t_coder *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

t_heap *create_heap(int capacity)
{
    t_heap *heap;
    t_coder **data;
    
    heap = (t_heap *)malloc(sizeof(t_heap));
    if (!heap)
        return NULL;
    
    data = (t_coder **)malloc(sizeof(t_coder *) * capacity);
    if (!data)
        return (free(heap), NULL);

    heap->capacity = capacity;
    heap->count = 0;
    heap->data = data;
    pthread_mutex_init(&heap->heap_mutex, NULL);
    return heap;
}

void bubble_down(t_heap *heap, int size, char *scheduler)
{
    int i;
    int left_child;
    int right_child;
    int small_child;
    int method;

    i = size;
    method = sorting_methode(scheduler);
    while ((i * 2 + 1) < heap->count)
    {
        left_child = i * 2 + 1;
        right_child = i * 2 + 2;
        small_child = left_child;

        if (method == 2)
        {
            if (right_child < heap->count &&
                heap->data[left_child]->arrived_time > heap->data[right_child] ->arrived_time )
                small_child = right_child;
            if (heap->data[small_child]->arrived_time < heap->data[i]->arrived_time )
                swap(heap->data + small_child, heap->data + i);
            else
                break;
        }
        else
        {
            if (right_child < heap->count &&
                heap->data[left_child]->last_compile > heap->data[right_child]->last_compile )
                small_child = right_child;
            if (heap->data[small_child]->last_compile < heap->data[i]->last_compile )
                swap(heap->data + small_child, heap->data + i);
            else
                break;
        }
        i = small_child;
    }
}

void bubble_up(t_heap *heap, int size, char* scheduler)
{
    int i;
    int parent;
    int method;

    i = size;
    method = sorting_methode(scheduler);
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (method == 2)
        {
            if (heap->data[parent]->arrived_time  > heap->data[i]->arrived_time )
                swap(heap->data + parent, heap->data + i);
            else
                break;
        }
        else
        {
            if (heap->data[parent]->last_compile  > heap->data[i]->last_compile )
                swap(heap->data + parent, heap->data + i);
            else
                break;
        }
        i = parent;
    }
}

int push(t_heap *heap, t_coder *coder, char *scheduler)
{
    pthread_mutex_lock(&heap->heap_mutex);
    if (heap->count == heap->capacity)
    {
        pthread_mutex_unlock(&heap->heap_mutex);
        fprintf(stderr, "Error: cannot push coder %d: heap is full\n", coder->id);
        return (1);
    }
    heap->data[heap->count] = coder;
    heap->count++;
    bubble_up(heap, heap->count-1, scheduler);
    pthread_mutex_unlock(&heap->heap_mutex);
    return (0);
}

t_coder *pop(t_heap *heap, char* scheduler)
{
    t_coder *coder;
    
    pthread_mutex_lock(&heap->heap_mutex);
    if (heap->count == 0)
    {
        pthread_mutex_unlock(&heap->heap_mutex);
        return (NULL);
    }

    coder = heap->data[0];
    heap->data[0] = heap->data[heap->count-1];
    heap->count--;
    bubble_down(heap, 0, scheduler);
    pthread_mutex_unlock(&heap->heap_mutex);
    return (coder);
}
