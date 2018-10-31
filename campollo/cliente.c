#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
	int sockfd;
	int len;
	struct sockaddr_in address;
	char serverAddress[20]; 
	int result;
	char ch = 'A';

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	printf("Dame la dirección del servidor\n");
	scanf("%s",serverAddress);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(serverAddress);
	address.sin_port = htons(9734);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit(0);

}