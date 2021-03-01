#include "../libs/network.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{

	char buffer[1024] = {0};
	int connfd;
	initialize_network(SERVER,PORT,ADDR);
	while(1){
		accept_connections();
		memset(buffer,0,1024);
		if(read_data(buffer,1024,&connfd)){
			printf("%d %s",connfd,buffer);
			write_data(connfd,buffer);
		}
	}
	closeConnections();
	return 0;
}
