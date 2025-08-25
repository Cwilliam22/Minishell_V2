/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:47:02 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:47:02 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	do_chir(t_exec *exec, char *path)
{
	int	status_oldpwd;
	int	status_pwd;

	status_oldpwd = update_oldpwd(exec);
	if (status_oldpwd == 0)
		return (print_error("cd", NULL, "failed to update OLDPWD"),
			GENERAL_ERROR);
	if (chdir(path) != 0)
		return (print_error("cd", path, strerror(errno)), GENERAL_ERROR);
	status_pwd = update_pwd(exec);
	if (status_pwd == 0)
		return (print_error("cd", NULL, "failed to update PWD"), GENERAL_ERROR);
	return (SUCCESS);
}

static int	handle_cd_home(t_exec *exec)
{
	char	*home_path;
	int		status;

	home_path = get_env_var("HOME", exec->shell->env);
	if (!home_path)
		return (print_error("cd", "HOME", "not set"), GENERAL_ERROR);
	status = do_chir(exec, home_path);
	free(home_path);
	return (status);
}

static int	handle_cd_previous(t_exec *exec)
{
	char	*oldpwd;
	int		status;

	oldpwd = get_env_var("OLDPWD", exec->shell->env);
	if (!oldpwd)
		return (print_error("cd", NULL, "OLDPWD not set"), GENERAL_ERROR);
	printf("%s\n", oldpwd);
	status = do_chir(exec, oldpwd);
	free(oldpwd);
	return (status);
}

static int	handle_cd_path(t_exec *exec, char *target)
{
	return (do_chir(exec, target));
}

/**
 * @brief Changes the current working directory (cd command).
 * 
 * @param exec Execution context containing command arguments
 * @return int 0 on success, 1 on failure
 */
int	builtin_cd(t_exec *exec)
{
	char	**args;
	int		nb_args;

	nb_args = exec->nb_arg;
	args = exec->shell->commands->args_expanded;
	if (!args[1] || ft_strcmp(args[1], "~") == 0
		|| ft_strcmp(args[1], "--") == 0)
		return (handle_cd_home(exec));
	if (nb_args > 2)
	{
		print_error("cd", NULL, "too many arguments");
		return (GENERAL_ERROR);
	}
	if (ft_strcmp(args[1], "-") == 0)
		return (handle_cd_previous(exec));
	return (handle_cd_path(exec, args[1]));
}
