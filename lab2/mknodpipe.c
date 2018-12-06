#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char* argv[]){

    char ch;
    int fdes,res;

    char msg_1[] = "pause\n";
    char msg_2[] = "quit\n";


    unlink("/tmp/music");
    res = mknod("/tmp/music", S_IFIFO|0666, 0);
    if (res<0) perror("fifo_not_ceated");

    fdes = open("/tmp/music", O_WRONLY);
    if (fdes<0) perror("fifo_cant_open");


    while(1)
    {
        printf("your command: \n");
        
        ch = getchar();
        getchar();
        
        switch(ch)
        {
            case 'p': 
                printf("paused\n");
                write(fdes, msg_1, sizeof(msg_1));
                break;
            case 'c': 
                printf("closed\n");
                write(fdes, msg_2, sizeof(msg_2));
                break;
            default:
                printf("wrong command\n");
                break;
        }
    }
    close(fdes);
    return 0;
}

