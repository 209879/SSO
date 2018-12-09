#include  <stdio.h>
#include  <signal.h>
#include  <stdlib.h>
#include <unistd.h>

static void sig_handler(int);

int main(void)
{

     if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        fprintf(stderr, "can't catch SIGUSR1");
     
     if (signal(SIGUSR2, sig_handler) == SIG_ERR)
        fprintf(stderr, "can't catch SIGUSR2");

     signal(SIGINT, sig_handler);
     
     while(1) 
          pause();

     return 0;
}

static void sig_handler(int sig)
{
     if (sig == SIGUSR1)
        printf("received SIGUSR1\n");
     else if (sig == SIGUSR2)
        printf("received SIGUSR2\n");
     else if(sig == SIGINT){
          char  c;

          signal(sig, SIG_IGN);
          printf("You pressed Ctrl-C.\n"
                 "Do you want to quit? [y/n] ");
          c = getchar();
          if (c == 'y' || c == 'Y')
               exit(0);
          else
               signal(SIGINT, sig_handler);
          getchar();
     }
     else
          fprintf(stderr, "received signal %d\n", sig);
}

