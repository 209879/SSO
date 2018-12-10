#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8080 
#define MAX 80

struct sockaddr_in servaddr; 

void chat(int sockfd) 
{ 
    char buff[MAX]; 
    int n, len; 
    while(1) { 
        bzero(buff, sizeof(buff)); 
        printf("Write a meesage : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 
        sendto(sockfd, buff, sizeof(buff),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr));
        bzero(buff, sizeof(buff)); 
        n = recvfrom(sockfd, buff, MAX,  
            MSG_WAITALL, (struct sockaddr *) &servaddr, 
            &len); 
        printf("from server : %s", buff); 
    } 
}

int main() { 
    int socket_fd; 
  
    if ( (socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation fail"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    chat(socket_fd);
    close(socket_fd); 

    return 0; 
} 