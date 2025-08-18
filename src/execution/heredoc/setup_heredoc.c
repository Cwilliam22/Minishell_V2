#include "minishell.h"

static int	first_and_last_quotes(const char *str)
{
	int first;
	int last;
	int	i;

	i = 0;
	printf("str : %s\n", str);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			first = i;
		printf("str[i] : %c\n", str[i]);
		i++;
	}
	printf("first : %d\n", first);
	printf("i : %d\n", i);
	i--;
	while (i >= 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
			last = i;
		printf("str[i] : %c\n", str[i]);
		i--;
	}
	printf("first : %d\n", first);
	printf("last : %d\n", last);
	if ((str[first] == str[last]) && (last != first))
	{
		if (str[first] == '\'')
			return (SIMPLE_QUOTED);
		if (str[first] == '\"')
			return (DOUBLE_QUOTED);
	}
	return (HALF_QUOTED);
}

static int	odd_quotes(int s_quote, int d_quote, const char *str)
{
	int result;

	result = first_and_last_quotes(str);
	if (result != HALF_QUOTED)
	{
		if (result == SIMPLE_QUOTED && s_quote % 2 == 0)
			return (SIMPLE_QUOTED);
		else if (result == DOUBLE_QUOTED && d_quote % 2 == 0)
			return (DOUBLE_QUOTED);
		else
			return (HALF_QUOTED);
	}
	else
		return (HALF_QUOTED);
}

static int	type_of_quote(const char *str)
{
	int	single_quote;
	int	double_quote;

	if (!str)
		return (0);
	single_quote = ft_iter_char(str, '\'');
	double_quote = ft_iter_char(str, '\"');
	if (single_quote == 0 && double_quote == 0)
		return (NO_QUOTED); 
	else if (single_quote % 2 == 0 && double_quote != 0)
		return (odd_quotes(single_quote, double_quote, str));
	else if (double_quote % 2 == 0 && single_quote != 0)
		return (odd_quotes(single_quote, double_quote, str));
	else if (single_quote % 2 == 0 && double_quote % 2 == 0)
		return (first_and_last_quotes(str));
	else if (single_quote % 2 == 0 && double_quote % 2 != 0)
		return (HALF_QUOTED);
	else if (single_quote % 2 != 0 && double_quote % 2 == 0)
		return (HALF_QUOTED);
	else if (single_quote % 2 != 0 && double_quote % 2 != 0)
		return (HALF_QUOTED);
	else
		return (0);
}

static int	unquote_delimiter(t_heredoc *heredoc)
{
	char	*tmp;

	if (heredoc->quoted_delimiter == DOUBLE_QUOTED)
	{
		tmp = ft_strtrim(heredoc->delimiter, "\"");
		if (!tmp)
			return (0);
		free(heredoc->delimiter);
		heredoc->delimiter = tmp;
	}
	if (heredoc->quoted_delimiter == SIMPLE_QUOTED)
	{
		tmp = ft_strtrim(heredoc->delimiter, "\'");
		if (!tmp)
			return (0);
		free(heredoc->delimiter);
		heredoc->delimiter = tmp;
	}
	return (1);
}

int	setup_for_heredoc(t_heredoc *heredoc)
{
	if (!heredoc || !heredoc->delimiter)
		return (0);
	heredoc->quoted_delimiter = type_of_quote(heredoc->delimiter);
	if (heredoc->quoted_delimiter != HALF_QUOTED)
	{
		if (heredoc->quoted_delimiter != NO_QUOTED)
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
