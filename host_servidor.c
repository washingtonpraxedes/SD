#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <pthread.h>

#define PORTA 3500
#define LEN 4096
#define HOSTS 4


struct sockaddr_in remoto;
struct sockaddr_in remoto2;
struct sockaddr_in local;
int statusCord = 1;





void enviar_dados(int socket, char buf[]);
int criar_socket();
void receber_dados(int socket, char buf[]);
void init_serv(){
	int i,j, porta;
	int sockfd, cliente;
	int len = sizeof(struct sockaddr_in), slen;

	//for(i=1;i<HOSTS;i++){
		local.sin_family = AF_INET;
		porta = PORTA;
		printf("porta %d\n",porta);
    		local.sin_port = htons(porta);
		local.sin_addr.s_addr = INADDR_ANY;
		memset(local.sin_zero,0,8);
		sockfd = inicializar_servidor(SOCK_STREAM,local, 20);

	//}

		//for(i = 1;i<HOSTS;i++){
			printf("chegou aqui\n");
			while(1){
			printf("Aceitando...\n");
			cliente = accept(sockfd,(struct sockaddr *)&remoto,&len);
			 if(cliente == -1)
 			{
        			perror(" accept\n");
        			exit(1);
    			}
		   	int pid = fork();
			if(pid<0){
				perror("Erro no fork\n");
				exit(1);
    			}
    			if(pid == 0){
				close(sockfd);
				printf("Cliente conectou\n");
				char buf[LEN], ip[30];
				receber_dados(cliente,buf);
				int id = atoi(buf);

				int len2 = sizeof(struct sockaddr_in);
	
				int sock = criar_socket();

				remoto2.sin_family = AF_INET;
				porta = PORTA;
				printf("porta: %d\n",porta);
   				remoto2.sin_port = htons(porta);
				sprintf(ip, "10.0.0.%d",id);
				printf("ip: %s\n",ip);
   				remoto2.sin_addr.s_addr = inet_addr(ip);
   				memset(remoto2.sin_zero,0x0,8);


				if(connect(sock,(struct sockaddr *)&remoto2,len2) == -1)
 				{
   		     			perror(" connect\n");
   		     			exit(1);
   	 			}
				else{
					memset(buf,0x0,LEN);
					sprintf(buf, "I'M ALIVE");
					enviar_dados(sock, buf);
				}

				
				
				
				
				//enviar_dados(cliente,5);
				exit(0);
    			}
			close(cliente);
			}
		
		//}


}
int inicializar_servidor(int tipo, struct sockaddr_in addr, int conexoes);



int main(){
	int id;

	printf("----------------------------BROKER INICIADO-------------------------\n\n");


	int i,j,q;
	int sockfd[HOSTS], cliente[HOSTS],sock[HOSTS];

    	int len = sizeof(struct sockaddr_in), slen;

	int len2 = sizeof(struct sockaddr_in), slen2;
        char buffer[4096];

	

	

	

	while(1){
	//for do servidor
		init_serv();
		//init_client();	
		
		//sleep(10);
	//for do cliente		
	
	   
	}
	printf("\nServidor encerrado.\n");
 
	close(sockfd);



}



void enviar_dados(int socket, char buf[]){



if(send(socket,buf,sizeof(buf),0)>0){
		printf("CODIGO %s\n",buf);

	}
	else{
		perror(" send\n");
		exit(1);
	}


}

int criar_socket(){
   int sock = socket(AF_INET, SOCK_STREAM, 0);
   if(sock == -1)
    {
        perror(" socket\n");
        exit(1);
    }
    else
        printf("Socket criado com sucesso 2\n");
	
	return sock;

}

void receber_dados(int socket, char buf[]){	

	//while(1){
		if(recv(socket,buf,LEN,0)>0)
        	{
        	   printf("Recebeu %s\n\n",buf);
        	}
		else
		{
		   perror(" recv\n");
		   exit(1);
		}
	//}

}

int inicializar_servidor(int tipo, struct sockaddr_in addr, int conexoes){
	int sock;
	sock = socket(addr.sin_family, tipo, 0);

        if(sock == -1)
    	{
        	perror(" socket\n");
        	exit(1);
    	}
    	else
    		printf("\nSocket criado com sucesso 1\n");
	
	if(bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
   	{
    		perror(" bind\n");
		close(sock);
    		exit(1);
   	}

	if(listen(sock, conexoes)<0)
    	{
  		perror(" listen");
		close(sock);
 		exit(1);

    	}
	return sock; 
	

}

void *thread_func(void *args){


}
