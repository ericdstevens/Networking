/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main()
{
	int sock, connected, bytes_recieved , true = 1;  
	char send_data [1024] , recv_data[1024];       

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}

	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);


	while(1)
	{  

		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		printf("\n I got a connection from (%s , %d)",
				inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		while (1)
		{
			printf("\n SEND (q or Q to quit) : ");
			gets(send_data);


//////////////////////// SENDING PROMPT //////////////////////////

			if (strcmp(send_data , "-l") == 0 )
			{
				char command[50];
				strcpy(command, "ls > temp1");
				system(command);  		 
			}
			else if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
			{
				send(connected, send_data,strlen(send_data), 0); 
				close(connected);
				break;
			}

			else
				send(connected, send_data,strlen(send_data), 0);  

			bytes_recieved = recv(connected,recv_data,1024,0);


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "-l") == 0)
			{
				
				printf("\nmaking temp file\n");
				char command[50];
				strcpy(command, "ls > temp1");
				system(command); 
				SendData(connected);
				strcpy(command, "rm -f temp1");
				system(command); 


			}


			else  if (recv_data[0] == '-' && recv_data[1] == 'g')
			{

				char data[100];
				sprintf(data, "%s", recv_data);
				char *filename;
				filename = strtok(data, " ");
				filename = strtok(NULL, " ");
				printf("understood g command: %s", filename);
				
				
				char command[50];
				 
				sprintf(&command, "cat %s > temp1", filename);
				printf("\n\n %s \n\n", command);
				system(command); 


				SendData(connected);
				strcpy(command, "rm -f temp1");
				system(command); 
			}


			else{ 
				printf("\n RECIEVED DATA = %s " , recv_data);
				fflush(stdout);
			}
		}
	}       

	close(sock);
	return 0;
}




int SendData(int connected){

	FILE *fp;
	char *filename = "temp1";
	int c;
	int n = 0;

	fp = fopen(filename,"r");
	if(fp == NULL){
		perror("fail to open temp");
	}
				
	do{
		c = fgetc(fp);
		if(feof(fp)){break;}
		char ch = (char) c;
		printf("%c",c);
		send(connected, &ch, 1, 0);  
					

	}while(1);	
	fclose(fp);







	return 0;
} 
