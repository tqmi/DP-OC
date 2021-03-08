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
void check_username(char * buff);
void ask_to_play(char * buff);
int check_confirm(char * buff);
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
		int input_type = handle_inputs(user,buffer,1024); // read inputs

		switch (input_type){
		
		case A_NO_ACTION: // no inputs read
			break;	
		
		case A_REFRESH  : // refresh asked from user
			if(get_state(user) == S_INIT){ // re-try connection
				if(initialize_network(CLIENT,PORT,ADDR) == 0) 
					next_state = S_AUTH;
				else next_state = S_INIT;
			}
			else if(get_state(user) == S_MENU){ // reload avaialable players
				send_list_req();
			}
			break; 
		
		case A_INCORRECT: 
			if(get_state(user) == S_AUTH){ // server resonse for invalid username
				// next_state = S_AUTH;
				printMessage("Username already taken! please choose anothe one:");
			}
			else if(get_state(user) == S_PLAY){ //idk yet
				get_board(get_user_game(user),board);
				printBoard(board,"Invalid move!");
			}else if(get_state(user) == S_WAIT){ // other player unavailable or declined
				next_state = S_MENU;
			}
			break;	
		
		case A_USER_DATA: // user input
			if(get_state(user) == S_AUTH){ // if authenticating, chek for username availability
				check_username(buffer);
			}
			else if(get_state(user) == S_MENU){ // if in menu, ask for other players for a game
				ask_to_play(buffer);
			}
			else if(get_state(user) == S_CONF){ // if confirmation needed, check for a valid answer
				int conf = check_confirm(buffer);
				char msg[1024];memset(msg,0,1024);
				char *ans;
				if(conf) ans = "1";
				else ans = "0";
				compose_message(msg,MV_GAME_REQ,get_username(user),ans);
				write_data(0,msg);	
			}
			else if(get_state(user) == S_PLAY){ // if playing, validate move
				handle_move(buffer);
			}else if(get_state(user) == S_ENDG){ // if confirmation needed, check for a valid answer
				int conf = check_confirm(buffer);
				if(conf) next_state = S_MENU;
				else next_state = S_EXIT;
			}
			break;	
		
		case A_CONF_REQ :
			if(get_state(user) == S_MENU){
				next_state = S_CONF;
			}
			break; 
		case A_QUIT     :
			next_state = S_EXIT;
			break; 
		case A_FORFEIT  :
			next_state = S_ENDG;
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
		case A_CORRECT :
			if(get_state(user) == S_AUTH){ // server confirmation for 
				next_state = S_MENU;
			}
			break; 
		case A_ST_GAME :
			next_state = S_PLAY;
			break;
		case A_OP_LEFT :
			next_state = S_ENDG;
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
			send_list_req();
			break;
		case S_WAIT:
			printMessage("Please wait for your opponent!");
			break;
		case S_CONF:
			printMessage("Do you want to play with ... ? (yes/no)");
			break;
		case S_PLAY:
			get_board(get_user_game(user),board);
			if(get_player_color(get_user_game(user),user) == WHITE)
				printBoard(board,"You are WHITE (left)");
			else
				printBoard(board,"You are BLACK (right)");
			break;
		case S_ENDG:
			printMessage("Game ended! Do you want to return to the menu? (yes/no)");
			break;
		case S_EXIT:
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

void get_user_list(char * buff,char **list){

}

void send_list_req(){
	char msg[1024];
	compose_message(msg,MV_AV_USERS,get_username(user),"");
	write_data(0,msg);
}

void check_username(char * buff){
	char msg[1024];
	char * uname = strtok(buff,"\n ");
	set_username(user,uname);
	compose_message(msg,MV_CONN_INIT,strtok(buff,"\n "),"");
	write_data(0,msg);
}

void ask_to_play(char * buff){
	char msg[1024];
	compose_message(msg,MV_PLAY_WITH,get_username(user),buff);
	write_data(0,msg);
	next_state = S_WAIT;
}

int check_confirm(char * buff){
	if(strcmp(buff,"yes\n") == 0){
		return 1;
	}
	else if(strcmp(buff,"no\n") == 0){
		return 0;
	}
	return 0;
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
	

}