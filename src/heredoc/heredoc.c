
#include "minishell.h"

// creer get_exec()

int	type_of_quote(const char *str)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	if (!str)
		return (0);
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] != '\'')
			single_quote++;
		if (str[i] != '\"')
			double_quote++;
		i++;
	}
	if (single_quote == 0 && double_quote == 0)
		return (NO_QUOTED);
	if (single_quote % 2 == 0 && double_quote % 2 == 0)
		return (QUOTED);
	else
		return (HALF_QUOTED);
}

int	unquote_delimiter(t_heredoc *heredoc)
{
	char	*tmp;

	tmp = ft_strtrim(heredoc->delimiter, "\'");
	if (!tmp)
		return (0);
	free(heredoc->delimiter);
	heredoc->delimiter = tmp;
	tmp = ft_strtrim(heredoc->delimiter, "\"");
	if (!tmp)
		return (0);
	free(heredoc->delimiter);
	heredoc->delimiter = tmp;
	return (1);
}

int	setup_for_heredoc(t_heredoc *heredoc)
{
	if (!heredoc || !heredoc->delimiter)
		return (0);
	heredoc->quoted_delimiter = type_of_quote(heredoc->delimiter);
	if (heredoc->quoted_delimiter != HALF_QUOTED)
	{
		if (heredoc->quoted_delimiter == NO_QUOTED)
		{
			if (!unquote_delimiter(heredoc))
				return (0);
		}
	}
	else
	{
		print_error(NULL, NULL, "heredoc delimiter not fully quoted");
		return (set_exit_status(2), 0);
	}
	return (1);
}

int	create_file(t_redir *redir)
{
	char	*filename;

	if (!redir || !redir->heredoc)
		return (0);
	filename = ft_strjoin("/tmp/heredoc_", ft_itoa(redir->heredoc->id));
	if (!filename)
		return (0);
	redir->heredoc->fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	free(filename);
	if (redir->heredoc->fd < 0)
	{
		print_error(NULL, NULL, "failed to create heredoc file");
		return (set_exit_status(1), 0);
	}
	return (1);
}

char	*handle_hd_var(char *line, int *i, t_env *env, char *result)
{
	int start;
	char *var_name;
	char *value;

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

char *expand_vars(char *line)
{
	int	i;
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

void expand_heredoc_content(t_redir *redir, char *line)
{
	char *new_line;
	if (redir->heredoc->quoted_delimiter)
	{
		new_line = ft_strdup(line);
		if (!new_line)
			return ;
	}
	else
		new_line = expand_vars(line);
	ft_putstr_fd(new_line, redir->heredoc->fd);
	ft_putstr_fd("\n", redir->heredoc->fd);
}

int	read_and_write_heredoc(t_redir *redir)
{
	char	*line;

	while (1)
	{
		printf("I'm in while(1) !!! \n");
		line = readline("> ");
		if (!line)
		{
			close(redir->heredoc->fd);
			// clean_up_all(exec)
			exit(EXIT_FAILURE);
		}
		if (ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		expand_heredoc_content(redir, line);
		free(line);
	}
	return (1);
}

int	process_hd(t_redir *redir)
{
	pid_t	pid;
	if (!redir || !redir->heredoc)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		printf("I'm in the child process \n");
		read_and_write_heredoc(redir);
		//close_and_exit();
	}
	return (1);
}

void	heredoc(t_redir *redir)
{
	t_redir *head;

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
