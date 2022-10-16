// Client side implementation of UDP client-server model
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
	
#define PORT 8080
#define MAXLINE 1024
	
int main(int argc, char* argv[]) {
	if(argc != 5) //check for correct usage
    return -1;

	// Storing user input
    int rec_port = atoi(argv[1]);
    int sen_port = atoi(argv[2]);
    float rtt = atof(argv[3]);
	int p = atoi(argv[4]);

	// Initialising the sockets
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in	 servaddr;
	clock_t start, end;
    double cpu_time_used;
	
	// Creating a socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
		
	// Adding server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
		
	// Variable initialisation
	int n, len, received_num;
	char* msg;
	int num = 0;
	bool lost = 0;

	// Loop for sending all the packets
	while(num < p){
		lost = 1;
		num++; //increment the next packet number
		asprintf(&msg, "Packet:%d\n", num); //creating the message
		printf("%s\n", msg);

		sendto(sockfd, (char *)msg, strlen(msg), //sending the message to the receiver
			2048, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));

		printf("Message sent.\n");
				
		start = clock(); //start the timer for the wait time

		// Polling loop to recive the acks
		while(1){
			n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
						MSG_DONTWAIT, (struct sockaddr *) &servaddr,
						&len);


			if(n > 0){
        		sscanf(buffer, "%*[^0123456789]%d", &received_num); //extract the packet number from the message
				if(received_num == num + 1){ //check if the ack is correct
					lost = 0;
					printf("Successful tx\n");
				}
				else{
					lost = 1; //consider packet lost if the ack is incorrect
				}
				break;
			}

			end = clock();
			if((((double) (end - start)) / CLOCKS_PER_SEC) > rtt){ //check for timeout 
				lost = 1; //consider the packet lost if an ack isn't received
				break;
			}

		}

		if(lost){
			num--; //reset num since the packet wasn't received
			printf("Packet lost\n");
			continue;
		}

		buffer[n] = '\0';
		printf("Receiver : %s\n", buffer);
	}
	close(sockfd); //close the socket
	return 0;
}
