/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:35 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/24 15:32:04 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*unquote_delimiter(char *result, char *str, int *index)
{
	char	*section;
	char	*processed;
	int		quote_end;

	quote_end = find_matching_quote(str, *index);
	if (quote_end == -1)
		return (free(result), NULL);
	section = ft_substr(str, *index + 1, quote_end - *index - 1);
	if (str[*index] == '"' || str[*index] == '\'')
		processed = ft_strdup(section);
	else
		return (free(section), free(result), NULL);
	result = join_and_free(result, processed);
	free(section);
	*index = quote_end + 1;
	return (result);
}

static char	*update_delimiter(t_heredoc *heredoc)
{
	char	*result;
	int		i;
	char	*str;
	char	tmp[2];

	if (!heredoc || !heredoc->delimiter)
		return (NULL);
	str = heredoc->delimiter;
	result = ft_strdup("");
	if (ft_strchr(str, '\'') || ft_strchr(str, '"'))
		heredoc->quoted_delimiter = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			result = unquote_delimiter(result, str, &i);
		else
		{
			tmp[0] = str[i];
			tmp[1] = '\0';
			result = join_and_free(result, ft_strdup(tmp));
			i++;
		}
	}
	return (free(heredoc->delimiter), result);
}

int	create_heredoc_file(t_redir *redir)
{
	char	*filename;
	char	*id;

	if (!redir || !redir->heredoc)
		return (0);
	redir->heredoc->delimiter = update_delimiter(redir->heredoc);
	id = ft_itoa(redir->heredoc->id);
	if (!id)
		return (1);
	filename = ft_strjoin("/tmp/heredoc_", id);
	if (!filename)
		return (free(id), 1);
	free(id);
	redir->heredoc->path = filename;
	redir->heredoc->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->heredoc->fd < 0)
	{
		perror("create heredoc file");
		free(filename);
		redir->heredoc->path = NULL;
		return (1);
	}
	return (0);
}
