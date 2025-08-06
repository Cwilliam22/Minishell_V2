/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:11:50 by wcapt             #+#    #+#             */
/*   Updated: 2025/08/03 04:11:07 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Y avait-il une raison de faire ft_strdup(getcwd) ?
int	update_pwd(t_exec *exec)
{
	char	*new_pwd;
	int		result;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (0);
	result = set_env_var("PWD", new_pwd, exec->shell->env, 0);
	free(new_pwd);
	return (result);
}

int	update_oldpwd(t_exec *exec)
{
	char		*curr_pwd;
	int			result;

	curr_pwd = get_env_var("PWD", exec->shell->env);
	if (!curr_pwd)
	{
		curr_pwd = getcwd(NULL, 0);
		if (!curr_pwd)
			return (0);
	}
	result = set_env_var("OLDPWD", curr_pwd, exec->shell->env, 0);
	free(curr_pwd);
	return (result);
}

int	builtin_pwd(t_exec *exec)
{
	char	*path;

	(void)exec;
	path = getcwd(NULL, 0);
	if (!path)
	{
		print_error("pwd", path, "The file or directory couldn't be "
			"determined");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}
