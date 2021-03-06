#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

	//terminar de implementar eleicao e o tempo de resposta

	// desenhar um fluxograma

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 3500
#define LEN 4096
#define HOSTS 5


struct sockaddr_in remoto2;
int ids[5] = {1,2,3,4,5};

int total,coordinator;

void enviar_dados(int socket, char buf[]);
int criar_socket();
struct sockaddr_in inicializar_socket(int sufixo_ip);
void receber_dados(int socket, char buf[]);
void init_client(int id, int ehCoordenador, int tipoMensagem){
	int i,j, porta;
	char ip[30];
	int sock;
	char buf[LEN];

	if(ehCoordenador){

		for(i=1;i<=HOSTS;i++){
			remoto2 = inicializar_socket(i);		
		
			int len2 = sizeof(struct sockaddr_in), slen2;

		
			printf("Conectando com o host %d...\n",i);
				
			sock = criar_socket();

			if(connect(sock,(struct sockaddr *)&remoto2,len2) == -1)
 			{
        			perror(" connect\n");
    			}
			else{
				
				printf("Conectou com o Host %d\n",i);
				if(!tipoMensagem){
					memset(buf,0x0,LEN);
					printf("Enviando mensagem de coordenador...\n");
					sprintf(buf,"0");
					enviar_dados(sock, buf);
				
					close(sock);

				}
				else{
					//terminar de implementar eleicao e o tempo de resposta
				}
	
			}
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
		
		clock_t start = clock();

		while(1){
			while(1){
				clock_t end = clock();
				printf("tempo %.2lf\n",((double)(clock()-start)/CLOCKS_PER_SEC));
				if((clock()-start)/CLOCKS_PER_SEC > 2){
				
				break;

			}
	
			}
			printf("Aceitando conexão do coordenador...\n");
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
				//close(sockfd);
				receber_dados(cliente,buf);
				if(strcmp(buf,"1")==0){
					memset(buf,0x0,LEN);
					receber_dados(cliente,buf);
					int id_host = atoi(buf);
					printf("----------------ELEIÇÃO--------------\n");
					if(id > id_host){
					
						printf("Respondendo...\n");
						memset(buf,0x0,LEN);	
						sprintf(buf,"OK");	
						enviar_dados(cliente,buf);
					}
				}
				else{
					memset(buf,0x0,LEN);
					printf("Coordenador conectou\n");
					receber_dados(cliente,buf);
					printf("%s\n",buf);
				//continue;
				
				
				//enviar_dados(cliente,5);
				//exit(0);
				}
    			}
			
			close(cliente);
			}

		printf("chegou aqui\n");
			//init_election();
	}	

	


}

int inicializar_servidor(int tipo, struct sockaddr_in addr, int conexoes);



int main(){
	int id;
	int ehCoordenador;	
	int coordenador_vivo = 0;

	

	printf("Digite o ID do host: \n");
	scanf("%d",&id);
	printf("É coordenador? (0/1)\n");
	scanf("%d",&ehCoordenador);

	printf("----------------------------HOST h%d-------------------------\n\n",id);
	
	if(ehCoordenador){
		while(!coordenador_vivo){
			printf("Enviando mensagem para os hosts\n");
			init_client(id, ehCoordenador);
			printf("Deseja derrubar o coordenador? (0/1)\n");
			scanf("%d",&coordenador_vivo);


		}

		
	}
	else{
		init_client(id, ehCoordenador);
	}
	
	
	
	

}


struct sockaddr_in inicializar_socket(int sufixo_ip){
	struct sockaddr_in remoto2;
	char ip[30];
	int porta;

	remoto2.sin_family = AF_INET;
	porta = PORTA;
	printf("porta: %d\n",porta);
   	remoto2.sin_port = htons(porta);
	sprintf(ip, "10.0.0.%d",sufixo_ip);
	printf("ip: %s\n",ip);
   	remoto2.sin_addr.s_addr = inet_addr(ip);
   	memset(remoto2.sin_zero,0x0,8);

	return remoto2;
} 

void start_election(int id){
	int i;
	struct sockaddr_in remoto;	
	int sock;
	int len2 = sizeof(struct sockaddr_in);
	char buf[LEN];
	
	for(i=1;i<=HOSTS;i++){
			remoto = inicializar_socket(i);
			sock = criar_socket();
			if(connect(sock,(struct sockaddr *)&remoto2,len2) == -1)
 			{
        			perror(" connect\n");
    			}
			else{
				memset(buf,0x0,LEN);
				sprintf(buf,"ELEICAO");
				enviar_dados(sock,buf);


				memset(buf,0x0,LEN);				
				int response = recv(sock,buf,LEN,0);
				if(response == 0) printf("Eh o coordenador\n");
				else printf("Esse não é o processo coordenador\n");
				close(sock);	
			}
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

int receber_dados(int socket, char buf[]){	
		int status = recv(socket,buf,LEN,0);
		if(status>0)
        	{
        	   printf("Recebeu %s\n\n",buf);
        	}
		else if(status<0)
		{
		   perror(" recv\n");
		   exit(1);
		}
		else{
			printf("conexao fechada\n");
		}

		return status;	

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
