/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:53:44 by dasanche          #+#    #+#             */
/*   Updated: 2025/09/12 16:45:24 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_pid(pid_t pid)
{
	if (pid == -1)
	{
		perror("Failed creating child process");
		exit(3);
	}
}

void	first_child(char **argv, int *pipe_fd, char **env)
{
	int	fd;

	fd = open_file_read(argv[1]);
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	command(argv[2], env);
}

void	second_child(char **argv, int *pipe_fd, char **env)
{
	int	fd;

	fd = open_file_write(argv[4]);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(fd);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	command(argv[3], env);
}

int	process_call(char **argv, int *pipe_fd, char **env)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	pid1 = fork();
	check_pid(pid1);
	if (pid1 == 0)
		first_child(argv, pipe_fd, env);
	pid2 = fork();
	check_pid(pid2);
	if (pid2 == 0)
		second_child(argv, pipe_fd, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else
		return (1);
}
