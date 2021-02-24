#include "../libs/network.h"
#include <stdio.h>
#define PORT 8080
#define ADDR "127.0.0.1"
int main(int argc, char const *argv[])
{
	int sock;
	printf("%d",connect_to_server(PORT,ADDR,&sock));
	write_data(sock,"Hello there");
	return 0;
}
