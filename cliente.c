#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
	
pthread_t tid;
int sockfd;

void *coneccion(void *arg)
{
	char nick[100];
	char msg[100];	
	while(1 == 1){
//		read(sockfd, &nick, sizeof(nick)*sizeof(char));
		read(sockfd, &msg, sizeof(msg)*sizeof(char));
		printf("\n%s\n", msg);
		printf("\nMessage: ");
	}

}

int main()
{
	int len;
	struct sockaddr_in address;
	int result;
	char* message;
	/* Crear un socket para el cliente */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	/* Nombrar el socket, de acuerdo con el server */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	char nick[100];
	char src_nick[100];
	char msg[100];
	char send[200] = "";
	printf("Nick:");
	fgets (nick, 100, stdin);
	strtok(nick, "\n");
//	printf("mandando nick...\n");	
	write(sockfd, &nick, sizeof(nick)*sizeof(char));
	pthread_create(&tid,NULL,coneccion,NULL);
	while(1==1) {
		printf("\nMessage:");
		fgets (msg, 100, stdin);
		strtok(msg, "\n");
		memset(send,0,200);
		if(strcmp(msg, "exit") == 0){
//			printf("te saco pues\n");
//			printf("mandando mensaje\n");
//			write(sockfd, &nick, sizeof(nick)*sizeof(char));
			strcat(send,"exit");
			write(sockfd, &send, sizeof(send)*sizeof(char));
			break;
		}
		else{
//			printf("mandando mensaje\n");
//			write(sockfd, &nick, sizeof(nick)*sizeof(char));
			strcat(send,nick);
			strcat(send,":");
			strcat(send,msg);
			write(sockfd, &send, sizeof(send)*sizeof(char));
		}
//		printf("leyendo mensaje repetido\n");
	}
	close(sockfd);
	exit(0);
}
