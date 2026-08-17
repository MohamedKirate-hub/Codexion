/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkirat <mkirat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:30:08 by mkirate           #+#    #+#             */
/*   Updated: 2026/08/17 14:28:33 by mkirat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(int capacity)
{
	t_heap	*heap;
	t_coder	**data;

	heap = (t_heap *)malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	data = (t_coder **)malloc(sizeof(t_coder *) * capacity);
	if (!data)
	{
		free(heap);
		return (NULL);
	}
	if (pthread_mutex_init(&heap->heap_mutex, NULL))
	{
		free(heap);
		free(data);
		return (NULL);
	}
	heap->capacity = capacity;
	heap->count = 0;
	heap->data = data;
	return (heap);
}

void	bubble_down(t_heap *heap, int size, char *scheduler)
{
	int	i;
	int	left_child;
	int	right_child;
	int	small_child;
	int	method;

	i = size;
	method = sorting_methode(scheduler);
	while ((i * 2 + 1) < heap->count)
	{
		left_child = i * 2 + 1;
		right_child = i * 2 + 2;
		small_child = left_child;
		if (right_child < heap->count
			&& compare_bubble_down(heap->data[left_child],
				heap->data[right_child], method))
			small_child = right_child;
		if (compare_bubble_down(heap->data[i], heap->data[small_child], method))
		{
			swap(heap->data + small_child, heap->data + i);
			i = small_child;
		}
		else
			break ;
	}
}

void	bubble_up(t_heap *heap, int size, char *scheduler)
{
	int	i;
	int	parent;
	int	method;

	i = size;
	method = sorting_methode(scheduler);
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (compare_bubble_up(heap->data[parent], heap->data[i], method))
			swap(heap->data + parent, heap->data + i);
		else
			break ;
		i = parent;
	}
}

int	push(t_heap *heap, t_coder *coder, char *scheduler)
{
	pthread_mutex_lock(&heap->heap_mutex);
	if (heap->count == heap->capacity)
	{
		pthread_mutex_unlock(&heap->heap_mutex);
		fprintf(stderr, "Error: cannot push coder %d to heap\n", coder->id);
		return (-1);
	}
	heap->data[heap->count] = coder;
	heap->count++;
	bubble_up(heap, heap->count - 1, scheduler);
	pthread_mutex_unlock(&heap->heap_mutex);
	return (0);
}

t_coder	*pop(t_heap *heap, char *scheduler)
{
	t_coder	*coder;

	pthread_mutex_lock(&heap->heap_mutex);
	if (heap->count == 0)
	{
		pthread_mutex_unlock(&heap->heap_mutex);
		return (NULL);
	}
	coder = heap->data[0];
	heap->data[0] = heap->data[heap->count - 1];
	heap->count--;
	bubble_down(heap, 0, scheduler);
	pthread_mutex_unlock(&heap->heap_mutex);
	return (coder);
}
