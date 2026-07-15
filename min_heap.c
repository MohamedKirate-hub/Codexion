/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirate <mkirate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:30:08 by mkirate           #+#    #+#             */
/*   Updated: 2026/07/15 11:08:37 by mkirate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

typedef struct s_heap
{
    int *data;
    int capacity;
    int count;
} t_heap;

void swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

t_heap *create_heap(int capacity)
{
    t_heap *heap;
    int *data;

    heap = (t_heap *)malloc(sizeof(t_heap));
    if (!heap)
        return (NULL);
    data = (int *)malloc(sizeof(int) * capacity);
    if (!data)
        return (free(heap), NULL);

    heap->data = data;
    heap->capacity = capacity;
    heap->count = 0;

    return (heap);
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

        if (right_child < heap->count && heap->data[left_child] > heap->data[right_child])
            small_child = right_child;
        if (heap->data[small_child] < heap->data[i])
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
        parent = i / 2;
        if (heap->data[parent] > heap->data[i])
            swap(heap->data + parent, heap->data + i);
        else
            break;
        i = parent;
    }
}

void push(t_heap *heap, int value)
{
    if (heap->count == heap->capacity)
        return;
    heap->data[heap->count] = value;
    heap->count++;
    bubble_up(heap, heap->count-1);
}

int pop(t_heap *heap)
{
    int value;
    
    if (heap->count == 0)
        return (-1);

    value = heap->data[0];
    heap->data[0] = heap->data[heap->count-1];
    heap->count--;
    bubble_down(heap, 0);

    return (value);
}

void heap_sorter(int *arr, int capacity)
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

int     main(void)
{
    int arr[6] = {9, 0, 8, 11, 22, 4};
    heap_sorter(arr, 6);
    for (int i = 0; i < 6; i++)
        printf("%d\n", arr[i]);
    return (0);
}