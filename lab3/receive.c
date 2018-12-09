#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf {
  long mtype;
  char mtext[64];
} message_buff;

int
main(void)
{
  int msg_queue;   
  key_t k = ftok("/var", 'c');
  message_buff msg_1;
  
  if(k != -1) { 
    msg_queue = msgget(k, 0644 | IPC_CREAT);
    if(msg_queue != -1) {
      //char buff[64] = ""; 

      while(1) { 

        if(msgrcv(msg_queue, &msg_1, sizeof(msg_1), 1, 0) != -1) {
          
          if(strcmp(msg_1.mtext, "stop") == 0) 
            break;
          printf("%s typ: %ld \n", msg_1.mtext, msg_1.mtype); 
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