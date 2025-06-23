#include "pipex.h"

/* Pid = Process Identifier
   Identificador único asignado a cada proceso en ejecución.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   File descriptor      Significado
   0                    stdin  (entrada estándar)
   1                    stdout (salida estándar)
   2                    stderr (error estándar)
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	La función exit -> Termina la ejecución del programa inmediatamente
   - Esta función cierra cualquier recurso abierto y termina el programa con el código de salida indicado.
   - Un código de salida de 0 indica éxito, mientras que cualquier otro valor (usualmente 1) indica un error.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   Las funciones -> perror() y strerror(errno) se usan para mostrar errores relacionados con el sistema operativo.
   - perror() imprime un mensaje de error detallado basado en el valor de errno.
   - strerror(errno) devuelve una cadena de texto que describe el error basado en el valor de errno.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   La variable errno -> Es una variable global que almacena el código de error de la última operación del sistema que falló.
   - Cada vez que ocurre un error en las funciones del sistema, el valor de `errno` se actualiza para reflejar el tipo de error.
   - Las funciones `perror()` y `strerror()` utilizan el valor de `errno` para proporcionar descripciones detalladas de lo que salió mal.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   La función -> pipe() es una llamada al sistema en sistemas Unix/Linux que permite crear un canal de comunicación unidireccional entre dos procesos.
   Este canal, llamado pipe anónimo, se comporta como una tubería: un proceso puede escribir datos en un extremo y otro proceso puede leerlos desde el otro extremo.
	
   Sintaxis: 
   int pipe(int pipefd[2]);

   Parámetro: pipefd es un arreglo de 2 enteros:
   -pipefd[0]: descriptor de archivo para lectura.
   -pipefd[1]: descriptor de archivo para escritura.

   Valor de retorno:
   -Devuelve 0 si se crea el pipe con éxito.
   -Devuelve -1 si ocurre un error y establece errno.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
   La función -> fork() es una llamada al sistema en Unix/Linux que crea un nuevo proceso duplicando el proceso actual.
   El proceso original se llama padre y el nuevo proceso se llama hijo.
   Es decir, fork() clona el proceso actual.

   El sistema operativo:
   - Crea un nuevo proceso casi idéntico al actual:
   - Mismo código, variables, punteros, archivos abiertos, entorno, etc.
   - Pero con un PID (Process ID) diferente.

   Ambos procesos (padre e hijo) continúan ejecutándose desde la siguiente línea después del fork().

   Valor de retorno de fork()		En qué proceso se recibe	        Qué significa
   > 0 (PID del hijo)	        	Proceso padre	                    Es el PID del proceso hijo recién creado
   0	                        	Proceso hijo	                    Está dentro del proceso hijo
   -1	                        	Ambos	                            fork() falló (por ejemplo, no hay memoria o procesos disponibles)
*/
void	exec(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_path(s_cmd[0], env);
	if (execve(path, s_cmd, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		ft_free_tab(s_cmd);
		exit(0);
	}
}

void	child(char **argv, int *p_fd, char **env)
{
	int		fd;

	fd = open_file_read(argv[1]);
	dup2(fd, 0);
	dup2(p_fd[1], 1);
	close(p_fd[0]);
	exec(argv[2], env);
}

void	parent(char **argv, int *p_fd, char **env)
{
	int		fd;

	fd = open_file_write(argv[4]);
	dup2(fd, 1);
	dup2(p_fd[0], 0);
	close(p_fd[1]);
	exec(argv[3], env);
}

int main(int argc, char **argv, char **env)
{
    int     p_fd[2];        // Array para el pipe: p_fd[0] lectura, p_fd[1] escritura
    pid_t   pid;            // Variable para guardar el PID del hijo

    if (argc != 5)
        args_exit(1);   // Verifica que haya 5 argumentos, si no es así sale del programa con un mensaje por la salida de error estandar explicando el correcto funcionamiento.

    if (pipe(p_fd) == -1)
	{
		perror("Pipe creation error"); // Crea el pipe; si falla, sale con un mensaje de error
		exit(2);
	}

    if ((pid = fork()) == -1) // Crea un proceso hijo y guarda su PID
    {
		perror("Failed creating child process");
		exit(3);
	}        // Si fork falla, se termina el programa

    if (pid == 0)              // Si pid == 0, estás en el proceso hijo
        child(argv, p_fd, env);
    else                   // Si pid > 0, estás en el proceso padre
        parent(argv, p_fd, env);
	return (0);
}