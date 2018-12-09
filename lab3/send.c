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
      size_t length; 

      while(fgets(msg_1.mtext, sizeof(msg_1.mtext), stdin) != NULL) {
        if((length = strlen(msg_1.mtext+1)) > 0) { 
        	if(length > 5)
        		msg_1.mtype = 1;
        	else
        		msg_1.mtype = 2;
        		printf("typ: %ld\n", msg_1.mtype );


          if(msg_1.mtext [length - 1] == '\n') { msg_1.mtext [length - 1] = 0; length--; }
        
          if(msgsnd(msg_queue, &msg_1, length, IPC_NOWAIT) == -1) {
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