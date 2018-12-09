#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


char fifo_pipe[] = "/tmp/music";

void apply_command(char* cmd)
{
    int fdes = open(fifo_pipe, O_WRONLY);
    if (fdes<0) perror("fifo_cant_open");
    write(fdes, cmd, strlen(cmd));
    close(fdes);    
}

int run_mplayer()
{
    pid_t processId = fork();

    if (processId == 0)
    {
        printf("running mplayer\n");
        char cmd[256];
        snprintf(cmd, 256, "mplayer -quiet -fs -slave -idle -input file=%s", fifo_pipe);
        int status = system(cmd);
        exit(status);
    }
    else 
    {
        return processId;
    }
}

int main(int argc, char* argv[]){

    char ch;
    int res;

    char msg_1[] = "pause\n";
    char msg_2[] = "quit\n";
    char msg_3[] = "seek 3\n";


    unlink(fifo_pipe);
    res = mknod(fifo_pipe, S_IFIFO|0777, 0);
    if (res<0) perror("fifo_not_ceated");

    pid_t pid = run_mplayer();

    char file[20];
    char load[30] = "loadfile ";
    char end[] = " \n";

    strcpy(file, argv[1]);
    strcat(load, file);
    strcat(load, end);
    apply_command(load);


    while(1)
    {
        printf("your command: \n");
        
        ch = getchar();
        getchar();
        
        switch(ch)
        {
            case 'p': 
                printf("paused\n");
                apply_command(msg_1);
                break;
            case 'c': 
                printf("closed\n");
                apply_command(msg_2);
                break;
            case 's': 
                printf("seeked 3\n");
                apply_command(msg_3);
                break;
            default:
                printf("wrong command\n");
                break;
        }
    }
    return 0;
}

