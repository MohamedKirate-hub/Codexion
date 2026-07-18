/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:30:08 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/18 09:22:44 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void swap(t_heap_node **a, t_heap_node **b)
{
    t_heap_node *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

t_heap *create_heap(int capacity)
{
    t_heap *heap;
    t_heap_node **data;
    
    heap = (t_heap *)malloc(sizeof(t_heap));
    if (!heap)
        return NULL;
    
    data = (t_heap_node **)malloc(sizeof(t_heap_node *) * capacity);
    if (!data)
        return (free(heap), NULL);

    heap->capacity = capacity;
    heap->count = 0;
    heap->data = data;
    return heap;
}

void bubble_down(t_heap *heap, int size)
{
    int i;
    int left_child;
    int right_child;
    int small_child;

    i = size;
    while ((i * 2 + 1) < heap->count)
    {
        left_child = i * 2 + 1;
        right_child = i * 2 + 2;
        small_child = left_child;

        if (right_child < heap->count &&
            heap->data[left_child]->release_time > heap->data[right_child]->release_time)
            small_child = right_child;
        if (heap->data[small_child]->release_time < heap->data[i]->release_time)
            swap(heap->data + small_child, heap->data + i);
        else
            break;
        i = small_child;
    }
}

void bubble_up(t_heap *heap, int size)
{
    int i;
    int parent;

    i = size;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (heap->data[parent]->release_time > heap->data[i]->release_time)
            swap(heap->data + parent, heap->data + i);
        else
            break;
        i = parent;
    }
}

void push(t_heap *heap, t_heap_node *value)
{
    if (heap->count == heap->capacity)
        return;
    heap->data[heap->count] = value;
    heap->count++;
    bubble_up(heap, heap->count-1);
}

t_heap_node *pop(t_heap *heap)
{
    t_heap_node *value;
    
    if (heap->count == 0)
        return (NULL);

    value = heap->data[0];
    heap->data[0] = heap->data[heap->count-1];
    heap->count--;
    bubble_down(heap, 0);

    return (value);
}

void heap_sorter(t_heap_node **arr, int capacity)
{
    t_heap  *heap;
    int     i;

    i = 0;
    heap = create_heap(capacity);
    while (i < capacity)
        push(heap, arr[i++]);
    i = 0;
    while (i < capacity)
        arr[i++] = pop(heap);
}
