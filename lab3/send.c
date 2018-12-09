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
      size_t length; 

      while(fgets(buff, sizeof(buff), stdin) != NULL) {
        if((length = strlen(buff)) > 0) { 

          if(buff [length - 1] == '\n') { buff [length - 1] = 0; length--; }
        
          if(msgsnd(msg_queue, &buff, length, 0) == -1) {
            perror("msgsnd()");
            break;
          }
        }
      }
    } else {
      perror("msgget()");
      return 2;
    }

   
    if(msgctl(msg_queue, 0, NULL) == -1) {
      perror("msgctl()");
      return 3;
    }
  } else {
    perror("ftok()");
    return 1;
  }

  return 0;
}