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

struct sockaddr_in servaddr, cliaddr; 

void chat(int sockfd) 
{ 
    char buff[MAX]; 
    int n, len; 

    while(1) { 
        bzero(buff, MAX); 
  
        n = recvfrom(sockfd, buff, MAX,  
            MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
            &len); 

        printf("from client: %s\t to client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 

        while ((buff[n++] = getchar()) != '\n') ; 
  
        sendto(sockfd, buff, sizeof(buff),  
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len);
    } 
} 
  
int main() { 
    int socket_fd; 
      
    if ( (socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation fail"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    if ( bind(socket_fd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("binding fail"); 
        exit(EXIT_FAILURE); 
    } 
      
    chat(socket_fd);
      
    return 0; 
} 