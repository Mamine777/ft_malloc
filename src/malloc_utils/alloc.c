/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:47:48 by mokariou          #+#    #+#             */
/*   Updated: 2025/11/27 11:35:25 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_malloc.h"

void	*malloc_tiny(size_t size)
{

	t_block	*block = find_free_block(g_tiny_zones, size);
	if (!block)
	{
		t_zone *new_zone = create_tiny_zone();
		if (!new_zone)
			return NULL;
		block = new_zone->blocks;
		if (!block)
			return NULL;
	}
	block = split_block(block, size);
	block->free = false;
	return block_to_ptr(block);
}

void	*malloc_small(size_t size)
{

	t_block	*block = find_free_block(g_small_zones, size);
	if (!block)
	{
		t_zone *new_zone = create_small_zone();
		if (!new_zone)
			return NULL;
		block = new_zone->blocks;
		if (!block)
			return NULL;
	}
	block = split_block(block, size);
	block->free = false;
	return block_to_ptr(block);
}

void *malloc_large(size_t size)
{
    t_zone *new_zone = create_large_zone(size);
    if (!new_zone)
        return NULL;

    t_block *block = new_zone->blocks;
    block->free = false;

    return block_to_ptr(block);
}
