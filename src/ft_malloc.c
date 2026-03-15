/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:52:32 by mokariou          #+#    #+#             */
/*   Updated: 2026/03/15 14:37:19 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malloc.h"

t_zone *g_tiny_zones = NULL;
t_zone *g_small_zones = NULL;
t_zone *g_large_zones = NULL;
t_zone *g_zones = NULL;
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*ft_malloc(size_t size)
{
	size_t aligned_size;
	t_zone_type zone_type;

	if (size < 1)
		return NULL;
	aligned_size = aligned(size);
	zone_type = get_zone_type(aligned_size);
	pthread_mutex_lock(&g_malloc_mutex);
	if (zone_type == TINY)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return malloc_tiny(aligned_size);
	}
	else if (zone_type == SMALL)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return malloc_small(aligned_size);
	}
	else if (zone_type == LARGE)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return malloc_large(size);
	}
	return NULL;
}

