/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:05:28 by dasanche          #+#    #+#             */
/*   Updated: 2025/06/27 19:10:36 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void command(char *comand, char **env)
{
    char **comand_args;
    char **directories;

    comand_args = ft_split(comand, ' ');
    if (!comand_args || !comand_args[0])
    {
        ft_putstr_fd("pipex: invalid command\n", 2);
        ft_free_tokens(comand_args);
        exit(9);
    }
    directories = get_path_dirs(env);
    try_exec_from_paths(comand_args, directories, env);
    ft_putstr_fd("pipex: command not found: ", 2);
    ft_putstr_fd(comand_args[0], 2);
    ft_putstr_fd("\n", 2);
    ft_free_tokens(comand_args);
    ft_free_tokens(directories);
    exit(127);
}

char	**get_path_dirs(char **env)
{
	char	*path_variable;
	char	**directories;

	path_variable = get_path_variable(env);
	if (!path_variable)
	{
		perror("PATH variable not found");
		exit(4);
	}
	directories = ft_split(path_variable, ':');
	if (!directories || !directories[0])
	{
		perror("Error splitting PATH");
		exit(5);
	}
	return (directories);
}

void	exec_if_found(char *path, char **args, char **env)
{
	if (access(path, X_OK) == 0)
	{
		execve(path, args, env);
		perror("Execve failed");
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
		total_length = ft_strlen(directories[i])
			+ ft_strlen(cmd_args[0]) + 2;
		cmd_path = malloc(total_length);
		if (!cmd_path)
		{
			perror("Memory allocation failed");
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

int main(int argc, char **argv, char **env)
{
    int pipe_fd[2];

    if (argc != 5)
        args_exit(1);

    if (pipe(pipe_fd) == -1)
    {
        perror("Pipe creation error");
        exit(2);
    }

    process_call(argv, pipe_fd, env);
    return (0);
}
