
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdio.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
  
int main() 
{ 
    struct sockaddr_in servaddr, cli;

    int socket_fd, connection_fd, length; 
 

    socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    bzero(&servaddr, sizeof(servaddr)); 
  

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  

    bind(socket_fd, (SA*)&servaddr, sizeof(servaddr)); 
    listen(socket_fd, 5);
    length = sizeof(cli); 
  
    connection_fd = accept(socket_fd, (SA*)&cli, &length);  
    if (connection_fd >= 0)
        printf("success");
  
    close(socket_fd); 
} 
