
// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h> 
 #include <arpa/inet.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

int main() 
{ 
    int socket_fd, connection_fd; 
    struct sockaddr_in servaddr, cli; 

    socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_fd == -1) { 
        printf("socket not created \n"); 
        exit(0); 
    } 
    else
        printf("socket created \n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    if (connect(socket_fd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection fail \n"); 
        exit(0); 
    } 
    else
        printf("connected to server \n");

    pause();

    close(socket_fd); 
} 

