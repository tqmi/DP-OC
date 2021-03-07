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
#include <errno.h>
#include <limits.h>

void initialize_modules();
void deinitialize_modules();
void run_cyclic();

void send_list_req();
void check_username(char * buff);
void ask_to_play(char * buff);
void check_confirm(char * buff);
void handle_move(char * buff);
void handle_server_data(char * msg);

int running = 0;
t_user * user;
int next_state = 0;
char ** oplayers;
int list_refresh_req = 0;
int board[8][8];

int main(int argc, char const *argv[])
{

	initialize_modules();

	// handle_move("A 4 h 3");
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
	char buffer[1024];
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
				check_username(buffer);
			}
			else if(get_state(user) == S_MENU){
				ask_to_play(buffer);
			}
			else if(get_state(user) == S_CONF){
				check_confirm(buffer);	
			}
			else if(get_state(user) == S_PLAY){
				handle_move(buffer);
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
			handle_server_data(buffer);
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
			printMessage("MENU");
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

int get_int(char * buff){
	return strtol(buff,NULL,10);
}

void get_user_list(char * buff,char **list){

}

void send_list_req(){
	char msg[1024];
	compose_message(msg,MV_AV_USERS,get_username(user),"");
	write_data(0,msg);
}

void check_username(char * buff){
	char msg[1024];
	compose_message(msg,MV_CONN_INIT,buff,"");
	write_data(0,msg);
}

void ask_to_play(char * buff){
	char msg[1024];
	compose_message(msg,MV_PLAY_WITH,get_username(user),buff);
	write_data(0,msg);
	next_state = S_WAIT;
}

void check_confirm(char * buff){
	char msg[1024];
	char *ans;
	if(strcmp(buff,"yes\n") == 0){
		ans = "1";
	}
	else if(strcmp(buff,"no\n") == 0){
		ans = "0";
	}

	compose_message(msg,MV_GAME_REQ,get_username(user),ans);
	write_data(0,msg);
}

int validate_move(int x1,int x2, int x3, int x4){

	x1 = x1 - 10 + 'a';
	x3 = x3 - 10 + 'a'; 

	if(x1 < 'a' || x1 > 'h' || x3 < 'a' || x3 > 'h' || x2 < 1 || x2 > 8 || x4 < 1 || x4 > 8)
		return -1;

	if(movePiece(get_user_game(user),x1,x2,x3,x4))
	    return 1;
    return 0;
}

void handle_move(char * buff){
	if(get_turn(get_user_game(user)) == get_player_color(get_user_game(user),user)){
		char * start = buff;
		char * end;
		int x1 = strtoll(start,&end,20);
		start = end;
		int x2 = strtoll(start,&end,20);
		start = end;
		int x3 = strtoll(start,&end,20);
		start = end;
		int x4 = strtoll(start,&end,20);
		start = end;
		// wprintf(L"%d %d %d %d\n",x1,x2,x3,x4);
		if(validate_move(x1,x2,x3,x4)){
			get_board(get_user_game(user),board);
			printBoard(board,"Oponents turn");
		}
	}
	else return;
}
void handle_server_data(char * msg){
	int msg_type;
	char server[100];memset(server,0,100);
	char payload[1024];memset(payload,0,1024);
	msg_type = decompose_message(msg,server,payload);
	char users[100][100];
	if(strcmp(server,"server")!=0)
		return;
	
	switch(msg_type){
	case MV_CONN_INIT   :
		if(get_int(payload) && get_state(user) == S_AUTH) {
			send_list_req();
			next_state = S_MENU;
		}else if(get_state(user) == S_AUTH){
			next_state = S_AUTH;
		}
	break;
	case MV_AV_USERS    : 
		if(get_state(user) == S_MENU){
			get_user_list(payload,users);
			next_state = S_MENU;
		}
	break;
	case MV_GAME_REQ    :
		if(get_state(user) == S_MENU){
			if(get_int(payload) == 2){
				next_state = S_CONF;
			}
		} 
		else if(get_state(user) == S_WAIT){
			if(get_int(payload) == 0){
				next_state = S_MENU;
			}
			if(get_int(payload) == 1){
				// wait for color
			}
		}
	break;
	case MV_MAKE_MOVE   : 
		if(get_state(user) == S_PLAY){
			// get_board(get_user_game(user),board);
			// movePiece(board); 
		}
	break;
	case MV_SET_COLOR   :
		if(get_state(user) == S_WAIT || get_state(user) == S_CONF){
			// TODO initialize game
		} 
	break;
	case MV_PLAYER_LEFT : 
		if(get_state(user) == S_PLAY){
			// TODO player left
			next_state = S_MENU;
		}
	break;
	case MV_ILLEGAL_MOVE: 
		if(get_state(user) == S_PLAY){
			// TODO illegal move
		}
	break;
	}


}