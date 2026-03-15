/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 14:37:21 by mokariou          #+#    #+#             */
/*   Updated: 2026/03/15 14:37:23 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "../include/ft_malloc.h"

int main()
{
    printf("=== Testing ft_malloc, ft_free, ft_realloc ===\n");

    char *a = ft_malloc(50);
    if (!a)
    {
        printf("Malloc failed\n");
        return 1;
    }
    strcpy(a, "hello");
    printf("Allocated a: %p, contains: %s\n", a, a);

    ft_free(a);

    char *b = ft_malloc(50);
    printf("Allocated b: %p\n", b);

    if (a == b)
        printf("OK ✓ reused same block\n");
    else
        printf("FAILED ✗ did not reuse block\n");

    // Test realloc increasing size
    char *c = ft_realloc(b, 100);
    if (c)
    {
        printf("Realloc increased size: %p\n", c);
        strcpy(c + 50, " world");
        printf("After realloc: %s%s\n", c, c + 50);
    }

    // Test realloc shrinking size
    char *d = ft_realloc(c, 20);
    if (d)
        printf("Realloc shrunk size: %p\n", d);

    ft_free(d);

    return 0;
}