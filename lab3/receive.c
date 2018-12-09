#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int
main(void)
{
  int msg_queue;   
  key_t k = ftok("/var", 'c');
  
  if(k != -1) { 
    msg_queue = msgget(k, 0644 | IPC_CREAT);
    if(msg_queue != -1) {
      char buff[64] = ""; 

      while(1) { 

        if(msgrcv(msg_queue, &buff, sizeof(buff), 0, 0) != -1) {
          
          if(strcmp(buff, "stop") == 0) 
            break;
          printf("%s\n", buff); 
        } 
        else {
          perror("msgrcv()");
          break;
        }
      }
    } 
    else {
      perror("msgget()");
      return 2;
    }
  } 
  else {
    perror("ftok()");
    return 1;
  }

  return 0;
}