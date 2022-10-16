// Client side implementation of UDP client-server model
#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
	
#define PORT 8080
#define MAXLINE 1024
	
// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	char *hello = "Hello from sender";
	struct sockaddr_in	 servaddr;
	clock_t start, end;
    double cpu_time_used;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
		
	int n, len;
	int num = 0;
	char* msg;
	int p = 1;
	bool lost = 0;
	int num_rec;

	while(num < p){
		lost = 1;
		num++;
		asprintf(&msg, "%d message\n", num);
		printf("%s\n", msg);

		sendto(sockfd, (char *)msg, strlen(msg),
			2048, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));

		printf("message sent.\n");
				
		start = clock();
		while(1){
			n = recvfrom(sockfd, (char *)buffer, MAXLINE,
						MSG_DONTWAIT, (struct sockaddr *) &servaddr,
						&len);


			if(n > 0){
				if(atoi(&buffer[0]) == num + 1){ //get the seg number from the message
					lost = 0;
					printf("not lost\n");
				}
				else{
					lost = 1;
				}
				break;
			}

			end = clock();
			if((( (double) (end - start)) / CLOCKS_PER_SEC) > 1){
				printf("Did not receive a message :(\n");
				lost = 1;
				break;
			}

		}

		if(lost){
			num--;
			printf("lost\n");
			continue;
		}

		buffer[n] = '\0';
		printf("Receiver : %s\n", buffer);
	}
	close(sockfd);
	return 0;
}