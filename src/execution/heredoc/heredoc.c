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
	t_redir	*head;

	if (!redir)
		return ;
	head = redir;
	while (head)
	{
		if (head->type == REDIR_HEREDOC)
		{
			if (!setup_for_heredoc(head->heredoc))
				return ;
			if (!create_file(head))
				return ;
			if (!process_hd(head))
				return ;
		}
		head = head->next;
	}
}
