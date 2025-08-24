/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:22:47 by alexis            #+#    #+#             */
/*   Updated: 2025/08/25 00:30:44 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_split(char *expanded)
{
	char	**words;
	int		word_count;

	words = ft_split(expanded, ' ');
	if (!words)
		return (0);
	word_count = 0;
	while (words[word_count])
		word_count++;
	free_array(words);
	return (word_count > 1);
}

static int	count_words_in_expanded(char *expanded)
{
	char	**words;
	int		count;

	words = ft_split(expanded, ' ');
	if (!words)
		return (1);
	count = 0;
	while (words[count])
		count++;
	free_array(words);
	return (count);
}

static int	count_single_arg(char *arg, t_shell *shell)
{
	char	*expanded;
	int		count;

	expanded = handle_quotes(arg, shell);
	if (!expanded)
		return (0);
	if (is_quoted_arg(arg) || !should_split(expanded))
		count = 1;
	else
		count = count_words_in_expanded(expanded);;
	free(expanded);
	return (count);
}

static int	count_expanded_args(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	total;

	total = 0;
	i = 0;
	while (cmd->args[i])
	{
		total += count_single_arg(cmd->args[i], shell);
		i++;
	}
	return (total);
}

static void	add_split_args(char **final_args, int *k, char *arg, t_shell *shell)
{
	char	*expanded;
	char	**words;
	int		j;

	expanded = handle_quotes(arg, shell);
	if (!expanded)
		return ;
	if (!is_quoted_arg(arg) && expanded[0] == '\0')
	{
		free(expanded);
		return ;
	}
	if (is_quoted_arg(arg) || !should_split(expanded))
		final_args[(*k)++] = expanded;
	else
	{
		words = ft_split(expanded, ' ');
		if (words)
		{
			j = 0;
			while (words[j])
				final_args[(*k)++] = words[j++];
			free(words);
		}
		free(expanded);
	}
}

void	expand_command_args(t_cmd *cmd, t_shell *shell)
{
	char	**final_args;
	int		total_count;
	int		i;
	int		k;

	total_count = count_expanded_args(cmd, shell);
	if (total_count == 0)
	{
		cmd->args_expanded = NULL;
		return ;
	}
	final_args = malloc(sizeof(char *) * (total_count + 1));
	if (!final_args)
		return ;
	k = 0;
	i = 0;
	while (cmd->args[i])
		add_split_args(final_args, &k, cmd->args[i++], shell);
	final_args[k] = NULL;
	cmd->args_expanded = final_args;
}
