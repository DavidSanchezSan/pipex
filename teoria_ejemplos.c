
#include "pipex.h"

// int main(/*int argc, char **argv*/)
// {
//     int child_id;
//     int num;
//     int i;

//     child_id = fork();
//     if (child_id == 0)
//         num = 1;
//     else
//         num = 6;
    
//     if (child_id != 0)
//         wait(NULL);

//     i = 0;
//     while(i < num + 5)
//     {
//         if (i == 0 && child_id == 0)
//             printf("Initing child process\n");
//         if (i == 0 && child_id != 0)
//             printf("Initing parent process\n");
//         printf("%d\n", i);
//         fflush(stdout);
//         i++;
//         if ((i == 6) && (child_id == 0))
//             printf("Finished child process\n------------------------\n");
//     }
//     if(child_id != 0)
//         printf("Finished parent process\n");
//     return (0);
// }

// ----------------------------------------------------------------------------------------

// int main(/*int argc, char **argv*/)
// {
//     int id1 = fork(); // id1 = 0 / id2 = y
//     int id2 = fork(); // id1 = 0 / id2 = 0
//     if (id1 == 0)
//     {
//         if (id1 == 0)
//             printf("Child process 1\n");
//         if (id2 == 0)
//             printf("Child process 2\n");
//     }
//     while(wait(NULL) != -1 || errno != ECHILD)
//         printf("Waited for child procesess to finish\n");
//     return (0);
// }

// ----------------------------------------------------------------------------------------

// int main(/*int argc, char **argv*/)
// {
//     int arr[] = {1, 2, 3, 4, 1, 2, 7, 7};
//     int arra_size = sizeof(arr)/sizeof(arr[0]);
//     int start;
//     int end;
//     int fd[2];
//     if (pipe(fd) == -1)
//         return (1);
    
//     int id = fork();
//     if (id == -1)
//         return (2);
//     if (id == 0)
//     {
//         start = 0;
//         end = (arra_size/2);
//     }
//     else
//     {
//         start = arra_size/2;
//         end = arra_size;
//     }
//     int sum = 0;
//     int i;
//     for (i = start; i < end; i++)
//     {
//         sum += arr[i];
//     }
//     printf("Calculated partial sum: %d\n", sum);
//     if (id == 0)
//     {
//         close(fd[0]);
//         if (write(fd[1], &sum, sizeof(sum)) == -1)
//             return(3);
//         close(fd[1]);
//     }
//     else
//     {
//         int sum_from_child;
//         close(fd[1]);
//         if (read(fd[0], &sum_from_child, sizeof(sum_from_child)) == -1)
//             return(4);
//         close(fd[0]);

//         int total_sum = sum + sum_from_child;
//         printf("Total sum is: %d\n", total_sum);
//         wait(NULL);
//     }
//     return(0);
// }

// ----------------------------------------------------------------------------------------

// ping -c 5 google.com | grep    rtt 
//       | stdout         |   stdin ^ 
//       |--------------> pipe------| 

// int main(/*int argc, char **argv*/)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//         return (1);
    
//     int pid1 = fork();
//     if (pid1 < 0)
//         return (2);

//     if (pid1 == 0)
//     {
//         //child process 1 (ping)
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[0]);
//         close(fd[1]);
//         execlp("ping", "ping", "-c", "5", "google.com", NULL); //No retorna nada
//     }

//     int pid2 = fork();
//     if(pid2 < 0)
//         return (3);
    
//     if (pid2 == 0)
//     {
//         //Child process 2 (grep)
//         dup2(fd[0], STDIN_FILENO);
//         close(fd[0]);
//         close(fd[1]);
//         execlp("grep", "grep", "rtt", NULL);
//     }

//     close(fd[0]);
//     close(fd[1]);
    
//     waitpid(pid1, NULL, 0);
//     waitpid(pid2, NULL, 0);
//     return (0);
// }

// ----------------------------------------------------------------------------------------

// int main(/*int argc, char **argv*/)
// {
//     int fd[2];
//     // fd[0] -> Read
//     // fd[1] -> Write
//     if (pipe(fd) == -1)
//     {
//         printf("Error opening pipe");
//         return (1);
//     }
//     int id = fork();
//     if (id == -1)
//     {
//         printf("Error with fork\n");
//         return (4);
//     }
//     if (id == 0)
//     {
//         close(fd[0]);
//         int x;
//         printf("Input a number: ");
//         scanf("%d", &x);
//         if(write(fd[1], &x, sizeof(int)) == -1)
//         {
//             printf("Error with writting");
//             return (2);
//         }
//         close(fd[1]);
//     }
//     else
//     {
//         close(fd[1]);
//         int y;
//         if (read(fd[0], &y, sizeof(int)) == -1)
//         {
//             printf("Error with reading");
//             return (3);
//         }
//         y *= 3;
//         close(fd[0]);
//         printf("Number from child process x 3 = %d\n", y);
//     }
//     return(0);
// }

// ----------------------------------------------------------------------------------------

// int main(/*int argc, char**argv*/)
// {
//     if (mkfifo("myfifo1", 0777) == -1)
//     {
//         if (errno != EEXIST)
//         {
//             printf("Could not create fifo file\n");
//             return(1);
//         }
//     }
//     printf("Opening...\n");
//     int fd = open("myfifo1", O_WRONLY);
//     if (fd == -1)
//         return (3);
//     printf("Opened\n");
//     int x = 97;
//     if (write(fd, &x, sizeof(x)) == -1)
//     {
//         printf("Error opening fifo file");
//         return(2);
//     }
//     printf("Written");
//     close(fd);
//     printf("Closed");
//     return(0);
// }

// ----------------------------------------------------------------------------------------

/* Al abrir un archivo se genera un int que aplica al archivo abierto
0 stdin
1 stdout
2 stderror

cin/ scanf --> Stdin
cout/ printf --> Buffer --> Stdout --> Screen
perror --> stderror --> Screen

dup y dup2 son llamadas al sistema que permiten cambiar estos file descriptors
El sistema siempre lee de 0 y escribe desde 1 y 2
Si queremos que se lea desde otro sitio hay que desactivar stdin
dup(0) --> lo que hace es duplicar fd 0 (stdin) por lo que ahora se lee desde nuestro fd
dup2(savestdin, 0) Devolvemos al crear un duplicado a stdin


*/

// ----------------------------------------------------------------------------------------

