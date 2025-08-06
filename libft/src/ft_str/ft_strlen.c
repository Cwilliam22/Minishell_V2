/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:11:25 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/03 05:34:10 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief The strlen() function computes the length of the string s. The
 * strnlen() function attempts to compute the length of s, but never scans
 * beyond the first maxlen bytes of s.
 * @param s The string to compute the length.
 * @return The strlen() function returns the number of characters that
 * precede the terminating NUL character. The strnlen() function returns
 * either the same result as strlen() or maxlen, whichever is smaller.
*/
#include "libft.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (i);
	while (*s)
	{
		s++;
		i++;
	}
	return (i);
}
