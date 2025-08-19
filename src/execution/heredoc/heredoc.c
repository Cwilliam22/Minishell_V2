#include "minishell.h"

char	*handle_hd_var(char *line, int *i, t_env *env, char *result)
{
	int		start;
	char	*var_name;
	char	*value;

	start = *i;
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	var_name = ft_substr(line, start, *i - start);
	value = get_env_var(var_name, env);
	if (!value)
		return (free(var_name), NULL);
	result = join_and_free(result, value);
	return (result);
}

char	*dollar_case(char *line, int *i, char *result)
{
	char	*temp;

	(*i)++;
	if (line[*i] == '?')
	{
		temp = ft_itoa(get_shell(NULL)->env->last_exit_status);
		if (!temp)
			return (NULL);
		result = join_and_free(result, temp);
		(*i)++;
	}
	else if (ft_isalpha(line[*i]) || line[*i] == '_')
		result = handle_hd_var(line, i, get_shell(NULL)->env, result);
	else
		result = join_and_free(result, "$");
	return (result);
}

char	*expand_vars(char *line)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
	while (line[i])
	{
		if (line[i] == '$')
		{
			result = dollar_case(line, &i, result);
		}
		else
		{
			result = append_char_to_str(result, line[i]);
			i++;
		}
	}
	return (result);
}

void	handle_heredoc(t_redir *redir)
{
	t_redir	*current;
	t_redir	*new_redir;
	t_redir	*next;

	if (!redir)
		return ;
	current = redir;
	while (current)
	{
		next = current->next;
		if (current->type == REDIR_HEREDOC)
		{
			printf("DEBUG: Handling heredoc for delimiter: %s\n", current->heredoc->delimiter);
			if (!setup_for_heredoc(current->heredoc))
				return ;
			if (!create_file(current))
				return ;
			process_hd(current);
		   new_redir = create_redirection(REDIR_IN, current->heredoc->path);
			if (!new_redir)
				return ;
			new_redir->next = current->next;
			current->next = new_redir;
			free_heredoc(current->heredoc);
			current->heredoc = NULL;
			current = new_redir->next;
		}
		else
			current = next;
	}
}
