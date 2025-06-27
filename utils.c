/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:25:37 by dasanche          #+#    #+#             */
/*   Updated: 2025/06/27 18:44:21 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
Devuelve un puntero a la ruta del ejecutable
buscando la variable PATH en el entorno
y omitiendo el string PATH=
*/
char	*get_path_variable(char **env)
{
	int		i;
	char	*path_var;

	i = 0;
	path_var = NULL;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_var = env[i] + 5;
			break ;
		}
		i++;
	}
	return (path_var);
}

/*
Función que libera los elementos convertidos
en tokens y guardados en array de arrays.
*/

void	ft_free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
