#include "../libs/network.h"

#define PORT 8080

int main(int argc, char const *argv[])
{
	int sock;
	initialize_network(SERVER);
	initialize_socket(PORT,10,&sock);
	while(1){
		accept_connections();
		read_data();
	}
	return 0;
}
