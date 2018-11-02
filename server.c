#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CLIENT 10
int totcltes=0;
int client_sockfd[MAX_CLIENT];
pthread_t tid[MAX_CLIENT];
void *coneccion_clte(void *arg);
int main()
{
	for(int i=0; i<MAX_CLIENT;i++)
		client_sockfd[i] = -1;
	int server_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int parametro[MAX_CLIENT];
	int i;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);
	bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
	/* Crear una cola de conexiones */
	listen(server_sockfd, 5);
	while(totcltes<MAX_CLIENT)
	{
//		printf("server esperando, clientes atendidos %d\n",totcltes);
		/* Aceptar conexión */
		client_sockfd[totcltes] = accept(server_sockfd,(struct sockaddr *)
		&client_address, &client_len);
		/* Crea un hilo que atiende al nuevo cliente */
		parametro[totcltes]=totcltes;
		pthread_create(&tid[totcltes],NULL,coneccion_clte,(void *)
		&parametro[totcltes]);
		totcltes++;
	}
	for(i=0;i<MAX_CLIENT;i++)
		pthread_join(tid[i],NULL);
}
void *coneccion_clte(void *arg)
{
	printf("atendiendo\n");
	char nick[100];
	char src_nick[100];
	char msg[100];
	int *n=(int *) arg;
	int mynum=*n;
//	printf("escuchando en: %d, %d", mynum, client_sockfd[mynum]);
//	printf("leyendo nick\n");
	read(client_sockfd[mynum], &nick, sizeof(nick)*sizeof(char));
	printf("%s is online\n", nick);
//	sleep(5);
	while(1==1){
//		printf("escuchando en: %d, %d", mynum, client_sockfd[mynum]);
//		printf("leyendo mensaje\n");
//		read(client_sockfd[mynum], &nick, sizeof(nick)*sizeof(char));
		read(client_sockfd[mynum], &msg, sizeof(msg)*sizeof(char));
//		printf("msg: %s",msg );		
		if(strcmp(msg, "exit") == 0){
			printf("%s is offline\n", nick);
			break;
		}
		for(int i=0;i<MAX_CLIENT;i++)
		{
			if(i!=mynum && client_sockfd[i]>=0){
//				printf("enviando a %d\n",i);
//				printf("devolviendo mensaje\n");
//				write(client_sockfd[i], &nick, sizeof(nick)*sizeof(char));
				write(client_sockfd[i], &msg, sizeof(msg)*sizeof(char));
//				printf("esperando mensaje...\n");
			}
		}
	}

	close(client_sockfd[mynum]);
	return (NULL);
}
