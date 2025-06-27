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
void	command(char *commands, char **env)
{
	char	**command_tokens;
	char	*path;

	command_tokens = ft_split(commands, ' ');
	path = get_path(command_tokens[0], env);
	if (execve(path, command_tokens, env) == -1)
	{
		ft_putstr_fd("Error with comand: ", 2);
		ft_putendl_fd(command_tokens[0], 2);
		ft_free_tokens(command_tokens);
		exit(0);
	}
}

void	child(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_read(argv[1]); // Se abre el archivo para leer
	dup2(fd, 0); // Se duplica stdin, es decir se apunta al archivo desde el que queremos leer como si este fuera stdin y se cierra el 0.
	dup2(pipe_fd[1], 1); // 1 es el extremo de escritura del pipe que hemos creado. Cierra el 1 si estaba abierto (stdout) y se redirige todo lo que fuera a 1, el extremo de escritura
	close(pipe_fd[0]); // End of file sin errores
	command(argv[2], env); // Se ejecuta el comando/ programa
}

void	parent(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open_file_write(argv[4]);  // Se abre el archivo para escribir
	dup2(fd, 1); // Se duplica stdout, es decir se apunta al archivo al que queremos escribir como si este fuera stdout y se cierra el 1.
	dup2(pipe_fd[0], 0); // 0 es el extremo de lectura del pipe que hemos creado. Cierra el 0 si estaba abierto (stdin) y se redirige todo lo que fuera a 0, el extremo de lectura
	close(pipe_fd[1]); // End of file sin errores
	command(argv[3], env); // Se ejecuta el comando/ programa
}

int main(int argc, char **argv, char **env)
{
    int     pipe_fd[2];        // Array para el pipe: p_fd[0] lectura, p_fd[1] escritura
    pid_t   pid;            // Variable para guardar el PID del hijo

   if (argc != 5) // Programa, archivo 1 (entrada), comando 1, comando 2, archivo 2 (salida).
      args_exit(1);   // Verifica que haya 5 argumentos, si no es así sale del programa con un mensaje por la salida de error estandar explicando el correcto funcionamiento.

   if (pipe(pipe_fd) == -1) // Pipe que utiliza un archivo de entrada y uno de salida. Genera una tubería de comunicación en memoria. Array de dos ints.
	{
		perror("Pipe creation error"); // Crea el pipe; si falla, sale con un mensaje de error
		exit(2);
	}

   if ((pid = fork()) == -1) // Crea un proceso hijo y guarda su PID
   {
		perror("Failed creating child process");
		exit(3); // Si fork falla, se termina el programa
	}        

   if (pid == 0)              // Si pid == 0, actualmente estoy en el proceso hijo
      child(argv, pipe_fd, env);
   else                   // Si pid != 0, actualmente estoy en el proceso padre
   {
      parent(argv, pipe_fd, env);
      wait(NULL); // Espera a que acabe el proceso hijo
   }
	return (0);
}