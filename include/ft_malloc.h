/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:15:56 by mokariou          #+#    #+#             */
/*   Updated: 2026/01/02 12:16:25 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MALLOC_H
#define MALLOC_H



# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>
# include <unistd.h>
# include <pthread.h>
# include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

# define TINY_MAX 128
# define SMALL_MAX 4096

typedef enum e_zone_type 
{
	TINY,
	SMALL,
	LARGE
} t_zone_type;

/*
 * t_block represents a memory block managed by the allocator.
 * It stores the size of the user payload, a flag indicating if the block is free,
 * and pointers to the next and previous blocks in the zone's linked list.
 */
typedef struct s_block
{
	size_t size; /* user payload size */
	bool free;  /* available checker */
	struct s_block *next;
	struct s_block *prev;
} t_block;


typedef struct s_zone
{
	enum e_zone_type type; /* zone type: TINY, SMALL, LARGE */
	size_t size; /* total size of the zone */
	struct s_zone *next;
	struct s_zone *prev;
	t_block *blocks; /* pointer to the first block in the zone */
} t_zone;

extern t_zone *g_tiny_zones;
extern t_zone *g_small_zones;
extern t_zone *g_large_zones;

extern pthread_mutex_t g_malloc_mutex;

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);

t_block *split_block(t_block *block, size_t requested_size);
size_t aligned(size_t size);
t_zone *create_new_zone(t_zone_type type, size_t requested_size_if_large);
t_zone_type get_zone_type(size_t size);
t_block	*find_free_block(t_zone *zone, size_t size);
void	*block_to_ptr(t_block *block);
t_block	*ptr_to_block(void *ptr);
t_zone	**get_zone_head(t_zone_type type);
t_zone	*create_tiny_zone(void);
t_zone	*create_small_zone(void);
t_zone	*create_large_zone(size_t requested_size);
void	merge_with_next(t_block *block);
t_block *merge_with_prev(t_block *block);
void	*malloc_tiny(size_t size);
void	*malloc_small(size_t size);
void    *malloc_large(size_t size);
void 	print_memory_layout(t_zone *zone);
t_zone	*find_zone_for_block(t_block *block);




#endif