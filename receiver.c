#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORT 8080
#define MAXLINE 1024 
    
// Driver code 
int main(int argc, char* argv[]) { 
    if(argc != 4) //check for correct usage
        return -1;
    
    // Storing user input
    int rec_port = atoi(argv[1]);
    int sen_port = atoi(argv[2]);
    float p_drop = atof(argv[3]);

    // Initialising the sockets
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr; 
        
    // Creating a socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr));
        
    // Adding server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
        
    // Binding the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0)
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Variable initialisation
    int len, n, received_num; 
    bool drop;
    float random;
    int next_num = 1;
    len = sizeof(cliaddr);  //len is value/result 
    srand((unsigned int) time(NULL)); //initialise the random number generator
    
    // Infinite reception loop
    while(1){
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len);
        buffer[n] = '\0';

        char* msg;
        sscanf(buffer, "%*[^0123456789]%d", &received_num); //extract the recieved package
        
        drop = 0;
        if(received_num == next_num) //if the correct packet is recieved, generate a random number
            random = ((float) rand() / (float)(RAND_MAX));
        else
            random = 1;

        if(random > p_drop){ //send and ack if the packet isn't dropped, go back to the loop otherwise
            if(received_num == next_num) //if the packet was correct, increment the ack number
                next_num++;

            asprintf(&msg, "Acknowledgement:%d\n", next_num); //generate the ack

            printf("Sender : %s\n", buffer);
            sendto(sockfd, (char *)msg, strlen(msg),  //send the ack
                2048, (const struct sockaddr *) &cliaddr, 
                    len); 

            printf("%s\n", msg);
        }
    }
    return 0;
}