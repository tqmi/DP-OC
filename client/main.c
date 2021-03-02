#include "../libs/network.h"
#include "../libs/logic.h"
#include "UI.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void initialize_modules();
void deinitialize_modules();
void run_cyclic();
int running = 0;

int main(int argc, char const *argv[])
{

	initialize_modules();

	run_cyclic();

	deinitialize_modules();
	return 0;
}

void initialize_modules(){
	initialize_network(CLIENT,PORT,ADDR);
	
}

void deinitialize_modules(){
	closeConnections();
}

void run_cyclic(){
	running = 1;
	char msg[100];
	int connfd;
	while (running)
	{
		memset(msg,0,100);
		if(read_data(msg,100,&connfd) > 0)
			if(connfd == stdin->_fileno){
				if(strcmp("exit\n",msg) == 0)
					running = 0;
				write_data(0,msg);
			}
			else
				printf("%s",msg);
	}
}