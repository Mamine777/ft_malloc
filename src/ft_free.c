/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:38 by mokariou          #+#    #+#             */
/*   Updated: 2025/12/08 15:14:39 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

t_block *merge_blocks(t_block *first, t_block *second)
{
	first->size += sizeof(t_block) + second->size;
	first->next = second->next;
	if (second->next)
		second->next->prev = first;
	return first;
}

// HOW : we check if the block can fall between the start and end of the zone
t_zone *find_zone_for_block(t_block *block)
{
	t_zone *zone_lists[3];
	t_zone *zone;
	char *zone_start;
	char *zone_end;
	int i;

	i = 0;
	zone_lists[0] = g_tiny_zones;
	zone_lists[1] = g_small_zones;
	zone_lists[2] = g_large_zones;
	while (i < 3)
	{
		 zone = zone_lists[i];

        while (zone)
        {
            zone_start = (char *)zone;
            zone_end   = zone_start + zone->size;

            if ((char *)block > zone_start && (char *)block < zone_end)
                return zone;

            zone = zone->next;
        }
        i++;
	}
	return (NULL);
}
t_block *merge_with_prev(t_block *block)
{
    t_block *prev = block->prev;

    if (prev && prev->free)
    {
        prev->size += sizeof(t_block) + block->size;
        prev->next = block->next;
        if (block->next)
            block->next->prev = prev;
        return prev; // prev is now the merged block
    }
    return block;
}
void merge_with_next(t_block *block)
{
    t_block *next = block->next;

    if (next && next->free)
    {
        block->size += sizeof(t_block) + next->size;
        block->next = next->next;
        if (next->next)
            next->next->prev = block;
    }
}

static void remove_zone(t_zone_type type, t_zone *zone)
{
    t_zone **head = get_zone_head(type);

    if (zone->prev)
        zone->prev->next = zone->next;
    else
        *head = zone->next;    // zone was head

    if (zone->next)
        zone->next->prev = zone->prev;
}

//using munmap to free large zones directly
void	ft_free(void *ptr)
{
	t_block	*block;
	t_zone	*zone;

	if (ptr == NULL)
		return;
	pthread_mutex_lock(&g_malloc_mutex);
	block = ptr_to_block(ptr);
	zone = find_zone_for_block(block);
	if (!zone)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return;
	}
	if (zone->type == LARGE)
	{
		remove_zone(LARGE, zone);
        munmap(zone, zone->size);
        pthread_mutex_unlock(&g_malloc_mutex);
        return;
	}
	block->free = true;
	//merge with next block if free only for tiny and small zones
	block = merge_with_prev(block);
    merge_with_next(block);
	
	if (!block->prev && !block->next)
	{
		//remove the entire fvcking zone
		remove_zone(zone->type, zone);
        munmap(zone, zone->size);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
} 