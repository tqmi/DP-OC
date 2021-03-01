#include "../libs/network.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char const *argv[])
{

	char msg[100] = {0};
	int connfd;
	initialize_network(CLIENT,PORT,ADDR);
	while (1){
		memset(msg,0,100);
		if(read_data(msg,100,&connfd) > 0)
			if(connfd == stdin->_fileno)
				write_data(0,msg);
			else
				printf("%s",msg);
		// sleep(1);
	}
	closeConnections();
	return 0;
}
