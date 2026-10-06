/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:01:03 by dasanche          #+#    #+#             */
/*   Updated: 2025/09/12 11:01:03 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	command(char *comand, char **env)
{
	char	**comand_args;
	char	**directories;

	comand_args = ft_split(comand, ' ');
	if (!comand_args || !comand_args[0])
	{
		ft_putstr_fd("pipex: invalid command\n", 2);
		ft_free_tokens(comand_args);
		exit(9);
	}
	if (ft_strchr(comand_args[0], '/') != NULL)
	{
		exec_if_found(comand_args[0], comand_args, env);
		perror("Execve failed");
	}
	else
	{
		directories = get_path_dirs(env);
		try_exec_from_paths(comand_args, directories, env);
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(comand_args[0], 2);
		ft_free_tokens(directories);
	}
	ft_free_tokens(comand_args);
	exit(127);
}

char	**get_path_dirs(char **env)
{
	char	*path_variable;
	char	**directories;

	path_variable = get_path_variable(env);
	if (!path_variable)
	{
		ft_putstr_fd("PATH variable not found\n", 2);
		exit(4);
	}
	directories = ft_split(path_variable, ':');
	if (!directories || !directories[0])
	{
		ft_putstr_fd("Error splitting PATH\n", 2);
		exit(5);
	}
	return (directories);
}

void	exec_if_found(char *path, char **args, char **env)
{
	if (access(path, X_OK) == 0)
	{
		execve(path, args, env);
		perror("Execve failed\n");
		exit(7);
	}
}

void	try_exec_from_paths(char **cmd_args, char **directories, char **env)
{
	char	*cmd_path;
	int		total_length;
	int		i;

	i = 0;
	while (directories[i])
	{
		total_length = ft_strlen(directories[i]) + ft_strlen(cmd_args[0]) + 2;
		cmd_path = malloc(total_length);
		if (!cmd_path)
		{
			perror("Memory allocation failed\n");
			exit(6);
		}
		ft_strlcpy(cmd_path, directories[i], total_length);
		ft_strlcat(cmd_path, "/", total_length);
		ft_strlcat(cmd_path, cmd_args[0], total_length);
		exec_if_found(cmd_path, cmd_args, env);
		free(cmd_path);
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	int	pipe_fd[2];
	int	call;

	if (argc != 5)
		args_exit(1);
	if (pipe(pipe_fd) == -1)
	{
		perror("Pipe creation error\n");
		exit(2);
	}
	call = process_call(argv, pipe_fd, env);
	return (call);
}
