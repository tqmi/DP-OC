#include "../libs/network.h"
#include "../libs/logic.h"
#include "UI.h"
#include "../libs/state.h"
#include "../libs/messages.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

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
	int board[8][8];
	char msg[100];
	while(running){

		memset(buffer,0,1024);
		switch (get_state())
		{
		case S_INIT:
			set_state(S_AUTH);
			break;
		case S_AUTH:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				compose_message(msg,MV_CONN_INIT,buffer,"");
				write_data(0,msg);
				set_state(S_MENU);
			}
			break;
		case S_MENU:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				if(strcmp(buffer,"exit\n") == 0){
					set_state(S_END);
				}
				if(strcmp(buffer,"wait\n") == 0){
					set_state(S_WAIT);
				}
				if(strcmp(buffer,"conf\n") == 0){
					set_state(S_CONF);
				}
			}
			break;
		case S_WAIT:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				if(strcmp(buffer,"exit\n") == 0){
					set_state(S_END);
				}
			}
			break;
		case S_CONF:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				if(strcmp(buffer,"exit\n") == 0){
					set_state(S_END);
				}
				if(strcmp(buffer,"yes\n") == 0){
					set_state(S_PLAY);
				}if(strcmp(buffer,"no\n") == 0){
					set_state(S_MENU);
				}
			}
			break;
		case S_PLAY:
			if((rd = get_keyboard_input(buffer,1024)) > 0){
				if(strcmp(buffer,"exit\n") == 0){
					set_state(S_MENU);
				}
				else{
					if(getPlayerTurn() == get_player_color()){
						if(validate_move(buffer)){
							getBoard(board);
							printBoard(board);
						}
					}
				}
			}
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