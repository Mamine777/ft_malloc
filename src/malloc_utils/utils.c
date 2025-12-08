/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 12:23:57 by mokariou          #+#    #+#             */
/*   Updated: 2025/11/27 11:35:25 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_malloc.h"

//align the size to be multiple of pointer size
// in short words we cant locate size 13 bytes it has to be 16 or multiple of 8, bigman ting
size_t aligned(size_t size)
{
	size_t alignment;
	alignment = sizeof(void *);
	if (size % alignment == 0)
		return size;
	return (size + (alignment - (size % alignment)));
}

// we want to split the block to 2 blocks, a new blcok with the remaining size and the old block with the requested size
t_block *split_block(t_block *block, size_t requested_size)
{
	t_block *new_block;
	size_t total_size;
	size_t remaining_size;
	size_t alignment = 16;

	total_size = block->size;
	remaining_size = total_size - requested_size - sizeof(t_block);
	if (remaining_size < sizeof(t_block) + alignment)
	{
		block->free = false;
		return block;
	}
	
	new_block = (t_block *)((char *)block + sizeof(t_block) + requested_size);
	new_block->size = remaining_size;
	new_block->free = true;
	new_block->next = block->next;
	new_block->prev = block;
	if (new_block->next)
		new_block->next->prev = new_block;
	block->next = new_block;
	block->size = requested_size;
	block->free = false;
	return block;
		
}

t_zone_type get_zone_type(size_t size)
{
    if (size <= TINY_MAX)
        return TINY;
    else if (size <= SMALL_MAX)
        return SMALL;
    else
        return LARGE;
}

void *block_to_ptr(t_block *block)
{
    return (void *)(block + 1);
}

t_block *ptr_to_block(void *ptr)
{
	return (t_block *)ptr - 1;
}

t_zone **get_zone_head(t_zone_type type)
{
    if (type == TINY)
        return &g_tiny_zones;
    else if (type == SMALL)
        return &g_small_zones;
    else
        return &g_large_zones;
}
