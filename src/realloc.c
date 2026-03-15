/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:54:08 by mokariou          #+#    #+#             */
/*   Updated: 2026/03/15 14:39:22 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"
void	*ft_realloc(void *ptr, size_t size)
{
	t_block	*block;
	size_t	copy_size;
	void	*new_ptr;
	t_zone	*zone;
	t_zone_type zone_type;
	t_block *next;

	if (ptr == NULL)
		return ft_malloc(size);
	if (size == 0)
	{
		ft_free(ptr);
		return NULL;
	}

	pthread_mutex_lock(&g_malloc_mutex);
	block = ptr_to_block(ptr);
	zone = find_zone_for_block(block);
	if (!zone)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return NULL;
	}

	zone_type = get_zone_type(size);

	// Large allocation: always allocate new memory
	if (zone_type == LARGE)
	{
		new_ptr = ft_malloc(size);
		if (!new_ptr)
		{
			pthread_mutex_unlock(&g_malloc_mutex);
			return NULL;
		}
		copy_size = (block->size < size) ? block->size : size;
		memcpy(new_ptr, ptr, copy_size);
		ft_free(ptr);
		pthread_mutex_unlock(&g_malloc_mutex);
		return new_ptr;
	}

	size = aligned(size);

	// Shrinking block: split if possible
	if (size <= block->size)
	{
		block = split_block(block, size);
		pthread_mutex_unlock(&g_malloc_mutex);
		return block_to_ptr(block);
	}

	// Try to merge with next block if free and enough space
	next = block->next;
	if (next && next->free &&
		(block->size + sizeof(t_block) + next->size >= size))
	{
		merge_with_next(block);
		block = split_block(block, size);
		pthread_mutex_unlock(&g_malloc_mutex);
		return block_to_ptr(block);
	}

	// Allocate new block
	copy_size = block->size;
	pthread_mutex_unlock(&g_malloc_mutex);

	new_ptr = ft_malloc(size);
	if (!new_ptr)
		return NULL;

	memcpy(new_ptr, ptr, copy_size);
	ft_free(ptr);
	return new_ptr;
}