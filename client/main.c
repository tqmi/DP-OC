#include "../libs/network.h"
#include "../libs/logic.h"
#include "UI.h"
#include "../libs/state.h"
#include "../libs/messages.h"
#include "inputHandler.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

void initialize_modules();
void deinitialize_modules();
void run_cyclic();

void send_list_req();
void check_username();
void ask_to_play();
void check_confirm();
void handle_move();
void handle_server_data();

int running = 0;
t_user * user;
int next_state = 0;
char ** oplayers;
int list_refresh_req = 0;

int main(int argc, char const *argv[])
{

	initialize_modules();

	run_cyclic();

	deinitialize_modules();
	return 0;
}

void initialize_modules(){
	user = init_state_user();
	if(initialize_network(CLIENT,PORT,ADDR) == 0){
		next_state = S_AUTH;
	}
	init_UI();
	
}

void deinitialize_modules(){
	closeConnections();
	free(user);
}


void run_cyclic(){
	int running = 1;
	int rd = 0;
	char buffer[1024];
	int board[8][8];
	char msg[100];
	while(running){
		memset(buffer,0,1024);
		int input_type = handle_inputs(user,buffer,1024);

		switch (input_type){
		case A_NO_ACTION:
			break;	
		case A_REFRESH  :
			if(get_state(user) == S_INIT){
				if(initialize_network(CLIENT,PORT,ADDR) == 0) 
					next_state = S_AUTH;
				else next_state = S_INIT;
			}
			else if(get_state(user) == S_MENU){
				send_list_req();
			}
			break; 
		case A_INCORRECT:
			if(get_state(user) == S_AUTH){
				next_state = S_AUTH;
			}
			else if(get_state(user) == S_PLAY){
				get_board(get_user_game(user),board);
				printBoard(board,"Invalid move!");
			}
			break;	
		case A_USER_DATA:
			if(get_state(user) == S_AUTH){
				check_username();
			}
			else if(get_state(user) == S_MENU){
				ask_to_play();
			}
			else if(get_state(user) == S_CONF){
				check_confirm();	
			}
			else if(get_state(user) == S_PLAY){
				handle_move();
			}
			break;	
		case A_CONF_REQ :
			next_state = S_CONF;
			break; 
		case A_QUIT     :
			next_state = S_END;
			break; 
		case A_FORFEIT  :
			next_state = S_MENU;
			break; 
		case A_SER_DATA :
			handle_server_data();
			break; 
		case A_ERROR    :
			next_state = S_INIT;
			break; 
		case A_SER_DISC :
			next_state = S_INIT;
			break; 
		}

		switch (next_state)
		{
		case S_INIT:
			printMessage("Connectiong to the server failed or an error occured!\nType \"try again\" to retry connection or \"exit\" to exit!");
			break;
		case S_AUTH:
			printMessage("Please enter a username:");
			break;
		case S_MENU:
			break;
		case S_WAIT:
			printMessage("Please wait for your opponent!");
			break;
		case S_CONF:
			printMessage("Do you want to play with ... ? (yes/no)");
			break;
		case S_PLAY:
			get_board(get_user_game(user),board);
			printBoard(board,"You are ... ");
			break;
		case S_END:
			printMessage("Goodbye!");
			running = 0;
			break;
		
		default:
			break;
		}
		
		if(next_state >= 0)
			set_state(user,next_state);
		next_state = -1;
		
	}
}


void send_list_req(){}
void check_username(){}
void ask_to_play(){}
void check_confirm(){}
void handle_move(){}
void handle_server_data(){}