#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 3500
#define LEN 4096
#define HOSTS 5


struct sockaddr_in remoto2;


int total,coordinator;

void enviar_dados(int socket, char buf[]);
int criar_socket();
void receber_dados(int socket, char buf[]);
void init_client(int id, int ehCoordenador){
	int i,j, porta;
	char ip[30];
	int sock;
	char buf[LEN];

	if(ehCoordenador){
		remoto2.sin_family = AF_INET;
		porta = PORTA;
		printf("porta: %d\n",porta);
   		remoto2.sin_port = htons(porta);
		sprintf(ip, "10.0.0.1");
		printf("ip: %s\n",ip);
   		remoto2.sin_addr.s_addr = inet_addr(ip);
   		memset(remoto2.sin_zero,0x0,8);
	//}
	
		int len2 = sizeof(struct sockaddr_in), slen2;

		//for(i=1;i<HOSTS;i++){
		//	if(i!= id){
			printf("Conectando com o host %d...\n",i);
				
			sock = criar_socket();

			if(connect(sock,(struct sockaddr *)&remoto2,len2) == -1)
 			{
        			perror(" connect\n");
        			exit(1);
    			}
			else{
				
				printf("Conectou com o Broker\n");
				
				memset(buf,0x0,LEN);
				printf("Enviando mensagem de coordenador...\n");
				printf("Digite o id do host de destino:\n");
				scanf("%s",buf);
				enviar_dados(sock, buf);
				
				
				
				close(sock);
	
			}
	}
	else{
		struct sockaddr_in local, remoto;
		int sockfd, cliente;
		int len = sizeof(struct sockaddr_in);		
	
		local.sin_family = AF_INET;
		porta = PORTA;
		printf("porta %d\n",porta);
    		local.sin_port = htons(porta);
		local.sin_addr.s_addr = INADDR_ANY;
		memset(local.sin_zero,0,8);
		sockfd = inicializar_servidor(SOCK_STREAM,local, 20);
		
		while(1){
			printf("Aceitando conexão do broker...\n");
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
				printf("Broker conectou\n");
				receber_dados(cliente,buf);
				printf("%s\n",buf);

				
				
				//enviar_dados(cliente,5);
				exit(0);
    			}
			close(cliente);
			}
	}	

	


}

int inicializar_servidor(int tipo, struct sockaddr_in addr, int conexoes);



int main(){
	int id;
	int ehCoordenador;	
	int derrubar = 0;
	printf("Digite o ID do host: \n");
	scanf("%d",&id);
	printf("É coordenador?\n");
	scanf("%d",&ehCoordenador);
	printf("----------------------------HOST h%d-------------------------\n\n",id);
	
	if(ehCoordenador){
		while(!derrubar){
			init_client(id, ehCoordenador);
			printf("Você deseja derrubar o coordenador?\n");
			scanf("%d",&derrubar);
	
		}
	}
	else{
		init_client(id, ehCoordenador);
	}
	
	
	
	

}




void enviar_dados(int socket, char buf[]){



if(send(socket,buf,sizeof(buf),0)>0){
		printf("CODIGO ENVIADO\n");

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

		if(recv(socket,buf,LEN,0)>0)
        	{
        	   printf("Recebeu %s\n\n",buf);
        	}
		else
		{
		   perror(" recv\n");
		   exit(1);
		}

	

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
