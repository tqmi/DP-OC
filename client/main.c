#include "../libs/network.h"
#include <stdio.h>
#include <unistd.h>
#define PORT 8080
#define ADDR "127.0.0.1"
int main(int argc, char const *argv[])
{
	int sock;
	char msg[100] = {0};
	printf("%d",connect_to_server(PORT,ADDR,&sock));
	for(int i = 1 ; i <= 10 ; i ++){
		sprintf(msg,"Hello %d\n",i);
		write_data(sock,msg);
		sleep(1);
	}
	close(sock);
	return 0;
}
