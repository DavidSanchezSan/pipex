#ifndef PIPEX_H
# define PIPEX_H

# include <string.h>
# include <errno.h>
# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include<sys/types.h>
# include<sys/stat.h>
# include <fcntl.h>  
# include <stdlib.h>

int		open_file_read(char *file);
int		open_file_write(char *file);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char **env);
void	exec(char *cmd, char **env);
void	ft_free_tokens(char **tab);
void	args_exit(int error_num);

#endif