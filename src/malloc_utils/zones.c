/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zones.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:25:11 by mokariou          #+#    #+#             */
/*   Updated: 2025/11/27 11:35:25 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_malloc.h"

t_block	*find_free_block(t_zone *zone, size_t size)
{
	t_zone *current_zone;
	t_block *block;

	current_zone = zone;
	while (current_zone != NULL)
	{
		block = current_zone->blocks;
		while (block != NULL)
		{
			if (block->free && 
				block->size >= size)
			{
				return (block);
			}
			block = block->next;
		}
		current_zone = current_zone->next;
	}
	return NULL;
}

t_zone *create_tiny_zone(void)
{
	t_zone *new_zone;
	size_t zone_size;
	size_t page_size;
	size_t needed;
	t_block *block_start;

	needed = sizeof(t_zone) + 100 * (TINY_MAX + sizeof(t_block));
	page_size = getpagesize();
	zone_size = ((needed + page_size - 1) / page_size) * page_size;
	new_zone = mmap(NULL, zone_size,
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (new_zone == MAP_FAILED)
		return NULL;
	new_zone->type = TINY;
	new_zone->size = zone_size;
	new_zone->next = NULL;
	new_zone->prev = NULL;
	block_start = (t_block *)((size_t)new_zone + sizeof(t_zone));
	block_start->free = true;
	block_start->size = zone_size - sizeof(t_zone) - sizeof(t_block);
	block_start->next = NULL;
	block_start->prev = NULL;
	new_zone->blocks = block_start;
	if (g_tiny_zones == NULL)
		g_tiny_zones = new_zone;
	else
	{
		new_zone->next = g_tiny_zones;
		g_tiny_zones->prev = new_zone;
		g_tiny_zones = new_zone;
	}
	return (new_zone);
}

t_zone*	create_small_zone(void)
{
	t_zone *new_zone;
	size_t	page_size;
	size_t	zone_size;
	size_t	needed;
	t_block	*block_start;

	needed = sizeof(t_zone) + 100 * (SMALL_MAX + sizeof(t_block));
	page_size = getpagesize();
	zone_size = ((needed + page_size - 1) / page_size) * page_size;
	new_zone = mmap(NULL, zone_size,
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (new_zone == MAP_FAILED)
		return NULL;
	new_zone->type = SMALL;
	new_zone->size = zone_size;
	new_zone->next = NULL;
	new_zone->prev = NULL;
	block_start = (t_block *)((size_t)new_zone + sizeof(t_zone));
	block_start->free = true;
	block_start->size = zone_size - sizeof(t_zone) - sizeof(t_block);
	block_start->next = NULL;
	block_start->prev = NULL;
	new_zone->blocks = block_start;
	if (g_small_zones == NULL)
		g_small_zones = new_zone;
	else
	{
		new_zone->next = g_small_zones;
		g_small_zones->prev = new_zone;
		g_small_zones = new_zone;
	}
	return (new_zone);
}

// large zone is only for one block so the whole zone has one big block and it only used
// for large allocations and when the user asked for thats why its set to false
t_zone	*create_large_zone(size_t requested_size)
{
	t_zone	*new_zone;
	size_t	page_size;
	size_t	zone_size;
	size_t	needed;
	t_block	*block_start;

	needed = sizeof(t_zone)
	+ sizeof(t_block)
	+ aligned(requested_size);
	page_size = getpagesize();
	zone_size = ((needed + page_size - 1) / page_size) * page_size;
	new_zone = mmap(NULL, zone_size,
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	new_zone->type = LARGE;
	new_zone->size = zone_size;
	new_zone->next = NULL;
	new_zone->prev = NULL;
	block_start = (t_block *)((size_t)new_zone + sizeof(t_zone));
	block_start->free = false;
	block_start->size = zone_size - sizeof(t_zone) - sizeof(t_block);
	block_start->next = NULL;
	block_start->prev = NULL;
	new_zone->blocks = block_start;
	if (g_large_zones == NULL)
		g_large_zones = new_zone;
	else
	{
		new_zone->next = g_large_zones;
		g_large_zones->prev = new_zone;
		g_large_zones = new_zone;
	}
	return (new_zone);
}

t_zone *create_new_zone(t_zone_type type, size_t requested_size_if_large)
{
	switch (type)
	{
		case TINY:
			return (create_tiny_zone());
		case SMALL:
			return (create_small_zone());
		case LARGE:
			return (create_large_zone(requested_size_if_large));
		default:
			return (NULL);
	}
}