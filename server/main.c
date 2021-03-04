#include "../libs/network.h"
#include "./serverWorker.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{

	char buffer[1024] = {0};
	char buffer2[1024] = {0};
	int connfd;
	initialize_network(SERVER,PORT,ADDR);
	while(1){
		accept_connections();
		memset(buffer,0,1024);
		memset(buffer2,0,1024);
		int rd = 0;
		if((rd = read_data(buffer,1024,&connfd)) >= 0 && connfd >= 0){
			if(connfd >= 0 && rd == 0){
				printf("%d disconnected\n",connfd);
			}
			else{
				// printf("%d %s",connfd,buffer);
				requestHandler(buffer,buffer2);
				write_data(connfd,buffer2);
			}
		}
	}
	closeConnections();
	return 0;
}
