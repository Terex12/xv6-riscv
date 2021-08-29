#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main (int argc, char **argv)
{
    int fd[2];
    if (pipe(fd) < 0) {
        printf("pipe error\n");
        exit(1);
    }

    char buffer[50];
    int pid = fork();
    if (pid == -1)
    {
        printf("fork error\n");
        exit (1);
    }
    else if (pid == 0)
    {
        // Child write
        close(fd[0]);
        int i = 0;
        while (i < 1000){
            char *message = "1234567890";
            int length = strlen( message );
            //printf( "message %s length %d\n", tmessagemp, length);
            write(fd[1], message, length);
            i++;
            if (i % 64)
                printf(".. %d\n", i);
        }
        printf("child: exiting\n");
        exit (0);
    }
    else
    {
        // Parent read
        // sleep(10);
        close(fd[1]);
        int start = uptime();
        int count = -1;
        while( (count = read(fd[0], buffer, sizeof(buffer)-1) )> 0){
            if (count <= 0) {
                printf("read error %d\n", count);
                exit(1);
            }
            buffer[count] = '\0';
            wait ((int *) 0);
            printf( "Parent read message from child: %s\n", buffer );
        }
        
        
        printf("time %d \n", uptime()-start);
        exit (0);
    }
}

