/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:37:23 by dasanche          #+#    #+#             */
/*   Updated: 2025/06/30 09:55:58 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_read(argv[1]);
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	command(argv[2], env);
}

void	parent(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_write(argv[4]);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	command(argv[3], env);
}

void	process_call(pid_t pid, char **argv, int *pipe_fd, char **env)
{
	if (pid == 0)
		child(argv, pipe_fd, env);
	else
	{
		parent(argv, pipe_fd, env);
		wait(NULL);
	}
}
