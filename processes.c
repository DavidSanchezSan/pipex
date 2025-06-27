/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanche <dasanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:37:23 by dasanche          #+#    #+#             */
/*   Updated: 2025/06/27 19:52:18 by dasanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Función que gestiona el proceso hijo,
abre el archivo para leer, se duplica stdin
(apuntamos al archivo desde el que queremos leer
como si este fuera stdin y se cierra el 0)
Cerramos el 1 (stdout) y lo sustituimos o redirigimos
a la salida del pipe creado. Cerramos el extremo 0
para lograr un endoffile sin errores y ejecutamos el
comando.
*/
void	child(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_read(argv[1]);
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	command(argv[2], env);
}

/*
Función que gestiona el proceso padre,
abre el archivo para escribir,
se duplica stdout, es decir se apunta al archivo
al que queremos escribir como si este fuera stdout
y se cierra el 1.
0 es el extremo de lectura del pipe que hemos creado.
Cierra el 0 si estaba abierto (stdin) y se redirige todo
lo que fuera a 0, el extremo de lectura
se cierra el extremo para tener un end of file sin errores
y se ejecuta el comando/ programa
*/
void	parent(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_write(argv[4]);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	command(argv[3], env);
}

/*
Función auxiliar para llamar a los dos procesos en
función del valor que tiene el identificador pid.
Si pid == 0, actualmente estoy en el proceso hijo
Si pid != 0, actualmente estoy en el proceso padre
Espera a que acabe el proceso hijo
*/
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
