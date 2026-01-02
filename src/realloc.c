/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:54:08 by mokariou          #+#    #+#             */
/*   Updated: 2026/01/02 12:18:13 by mokariou         ###   ########.fr       */
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
	if (zone_type == LARGE)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		void *new_ptr = ft_malloc(size);
		if (!new_ptr)
			return NULL;
		memcpy(new_ptr, ptr, block->size);
		ft_free(ptr);
		return (new_ptr);
	}
	// in the case of shrinking the block
	if (aligned(size) <= block->size)
	{
		block = split_block(block, aligned(size));
		pthread_mutex_unlock(&g_malloc_mutex);
		return block_to_ptr(block);
	}
	next = block->next;
	// try to merge with next block if it's free and has enough space
	if (next && next->free &&
        block->size + sizeof(t_block) + next->size >= aligned(size))	
	{
		merge_with_next(block);
		block = split_block(block, aligned(size));
		pthread_mutex_unlock(&g_malloc_mutex);
		return block_to_ptr(block);
	}
	//alocate new block
	copy_size = (block->size < size) ? block->size : size;
	pthread_mutex_unlock(&g_malloc_mutex);

	new_ptr = ft_malloc(size);
	if (!new_ptr)
		return NULL;
	memcpy(new_ptr, ptr, copy_size);
	ft_free(ptr);
	return (new_ptr);
}
