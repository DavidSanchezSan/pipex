#include "pipex.h"

void	args_exit(int error_num)
{
	ft_putstr_fd("Execution mode = ./pipex infile cmd cmd outfile\n", 2);
	exit(error_num);
}

int	open_file_read(char *file)
{
	int	fd_id;

	fd_id = open(file, O_RDONLY, 0777);
	if (fd_id == -1)
	{
		perror("Error opening file to read");
		exit(4);
	}
	return (fd_id);
}

int	open_file_write(char *file)
{
	int	fd_id;

	fd_id = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_id == -1)
	{
		perror("Error opening file to write");
		exit(5);
	}
	return (fd_id);
}

void	ft_free_tokens(char **tokens_to_free)
{
	int	i;

	i = 0;
	while (tokens_to_free[i])
	{
		free(tokens_to_free[i]);
		i++;
	}
	free(tokens_to_free);
}

char	*my_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *commands, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**command_tokens;

	i = -1;
	allpath = ft_split(my_getenv("PATH", env), ':');
	command_tokens = ft_split(commands, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, command_tokens[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tokens(command_tokens);
			return (exec);
		}
		free(exec);
	}
	ft_free_tokens(allpath);
	ft_free_tokens(command_tokens);
	return (commands);
}