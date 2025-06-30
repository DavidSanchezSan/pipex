/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:02:17 by dasanche          #+#    #+#             */
/*   Updated: 2025/06/27 15:12:53 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <string.h>
# include <errno.h>
# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>  
# include <stdlib.h>

void	args_exit(int error_num);
int		open_file_read(char *file);
int		open_file_write(char *file);
char	*get_path_variable(char **env);
void	ft_free_tokens(char **tokens);
void	child(char **argv, int *pipe_fd, char **env);
void	parent(char **argv, int *pipe_fd, char **env);
void	process_call(pid_t pid, char **argv, int *pipe_fd, char **env);
void	command(char *cmd, char **env);
char	**get_path_dirs(char **env);
void	exec_if_found(char *path, char **args, char **env);
void	try_exec_from_paths(char **cmd_args, char **directories, char **env);

#endif