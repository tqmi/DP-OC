#include "../libs/network.h"
#include "../libs/logic.h"
#include "UI.h"
#include "../libs/state.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>

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
	init_logic();
	init_UI();
	init_state();
}

void deinitialize_modules(){
	closeConnections();
}

void run_cyclic(){
	int running = 1;
	int rd = 0;
	char buffer[1024];
	while(running){
		switch (get_state())
		{
		case S_INIT:
			printMessage("Enter your username :");
			set_state(S_AUTH);
			break;
		case S_AUTH:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				write_data(0,buffer);
				memset(buffer,0,1024);
				set_state(S_MENU);
				printMessage("You are in the menu!\nType exit to exit!");
			}
			break;
		case S_MENU:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				if(strcmp(buffer,"exit\n") == 0){
					set_state(S_END);
				}
				memset(buffer,0,1024);
			}
			break;
		case S_WAIT:
			break;
		case S_CONF:
			break;
		case S_PLAY:
			break;
		case S_END:
			running = 0;
			break;
		
		default:
			break;
		}
	}


	// running = 1;
	// char msg[100];
	// int connfd;
	// while (running)
	// {
	// 	memset(msg,0,100);
	// 	if(read_data(msg,100,&connfd) > 0)
	// 		if(connfd == stdin->_fileno){
	// 			if(strcmp("exit\n",msg) == 0)
	// 				running = 0;
	// 			write_data(0,msg);
	// 		}
	// 		else
	// 			printf("%s",msg);
	// }
}