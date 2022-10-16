#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORT1 8080
#define PORT2 8082
#define MAXLINE 1024 
    
// Driver code 
int main(int argc, char* argv[]) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from the other side";
    struct sockaddr_in servaddr, cliaddr; 
        
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr));
        
    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT1); 
        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    int len, n, num; 
    int next_num;

    num = 12;

    len = sizeof(cliaddr);  //len is value/result 
    
    while(1){
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len);
        buffer[n] = '\0';

        char* msg;
        int received_num;
        sscanf(buffer, "%*[^0123456789]%d", &received_num);
        int next_num = received_num + 1;
        asprintf(&msg, "Acknowledgement:%d\n", next_num);

        printf("Sender : %s\n", buffer);
        sendto(sockfd, (char *)msg, strlen(msg),  
            2048, (const struct sockaddr *) &cliaddr, 
                len); 

        printf("%s\n", msg);
        printf("ack sent.\n");

    printf("%d %d\n", num, next_num);
        
    if(next_num == num + 1)
        break;
    }
    return 0;
}