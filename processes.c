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

void	first_child(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_read(argv[1]);
	dup2(fd, 0);            // stdin → infile
	dup2(pipe_fd[1], 1);    // stdout → pipe write end
	close(fd);               // cerrar infile
	close(pipe_fd[0]);       // cerrar pipe read end
	close(pipe_fd[1]);       // cerrar pipe write end después de duplicar
	command(argv[2], env);
}

void	second_child(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_write(argv[4]);
	dup2(fd, 1);            // stdout → outfile
	dup2(pipe_fd[0], 0);    // stdin → pipe read end
	close(fd);               // cerrar outfile
	close(pipe_fd[1]);       // cerrar pipe write end
	close(pipe_fd[0]);       // cerrar pipe read end después de duplicar
	command(argv[3], env);
}

void process_call(char **argv, int *pipe_fd, char **env)
{
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("Failed creating first child");
        exit(3);
    }
    if (pid1 == 0)
        first_child(argv, pipe_fd, env);
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("Failed creating second child");
        exit(3);
    }
    if (pid2 == 0)
        second_child(argv, pipe_fd, env);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
