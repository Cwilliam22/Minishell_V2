/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:04:44 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:04:44 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Allocates memory safely with error handling
* @param size Number of bytes to allocate
* @return Pointer to allocated memory, exits on failure
*/
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		print_error(NULL, NULL, "fatal: memory allocation failed");
		cleanup_and_exit(2, NULL);
	}
	return (ptr);
}
