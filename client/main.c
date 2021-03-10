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
void handle_game_request(char * msg);
void handle_server_move(char *msg);
void handle_forfeit();

t_user * user;
int next_state = 0;
char ** oplayers;
// int list_refresh_req = 0;
int board[8][8];
char o_uname[MSG_ID_SIZE];
char aux[1024] = {0};
int won = 0;
char msg[MSG_SIZE];
char server[MSG_ID_SIZE];
char payload[MSG_PAYLOAD_SIZE];
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
	char buffer[MSG_SIZE];
	while(running){

		memset(buffer,0,MSG_SIZE);
		int input_type = handle_inputs(user,buffer,MSG_SIZE); // read inputs

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
			}else if(get_state(user) == S_WAIT || get_state(user) == S_CONF){ // other player unavailable or declined
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
				memset(msg,0,MSG_SIZE);
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
				handle_game_request(buffer);
				next_state = S_CONF;
			}
			break; 
		case A_QUIT     :
			next_state = S_EXIT;
			break; 
		case A_FORFEIT  :
			if(get_state(user) == S_PLAY){
				handle_forfeit();
				next_state = S_ENDG;
			}
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
			if (get_state(user) == S_PLAY){
				won = 1;
				next_state = S_ENDG;
			}
			else if(get_state(user) == S_CONF || get_state(user) == S_WAIT)
				next_state = S_MENU;
			break;
		case A_LIST_USERS :
			if(get_state(user) == S_MENU){
				memset(server,0,MSG_ID_SIZE);
				memset(payload,0,MSG_PAYLOAD_SIZE);
				decompose_message(buffer,server,payload);
				printNameList(payload);
			}
			break;
		case A_MAKE_MOVE :
			handle_server_move(buffer);
			break;
		}

		switch (next_state)
		{
		case S_INIT:
			printMessage("Connectiong to the server failed or an error occured!\nType \"refresh\" to retry connection or \"exit\" to exit!");
			break;
		case S_AUTH:
			printMessage("Please enter a username:");
			break;
		case S_MENU:
			printMessage("Wait for opponents list. Type \"refresh\" to send request again!");
			send_list_req();
			break;
		case S_WAIT:
			printMessage("Please wait for your opponent!");
			break;
		case S_CONF:
			memset(aux,0,1024);
			strcat(aux,"Do you want to play with ");
			strcat(aux,o_uname);
			strcat(aux,"? (yes/no)");
			printMessage(aux);
			break;
		case S_PLAY:
			get_board(get_user_game(user),board);
			if(get_player_color(get_user_game(user),user) == WHITE)
				printBoard(board,"You are WHITE (left)");
			else
				printBoard(board,"You are BLACK (right)");
			break;
		case S_ENDG:
			if(won)
				printMessage("You WON! Do you want to return to the menu? (yes/no)");
			else
				printMessage("You LOST! Do you want to return to the menu? (yes/no)");
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

void handle_game_request(char * msg){
	
	memset(server,0,MSG_ID_SIZE);
	memset(payload,0,MSG_PAYLOAD_SIZE);

	decompose_message(msg,server,payload);
	char * tok = strtok(payload,",");
	tok = strtok(NULL,",");
	if(tok != NULL)
		strcpy(o_uname,tok);

}

void send_list_req(){
	memset(msg,0,MSG_SIZE);
	compose_message(msg,MV_AV_USERS,get_username(user),"");
	write_data(0,msg);
}

void check_username(char * buff){
	memset(msg,0,MSG_SIZE);
	char * uname = strtok(buff,"\n ");
	set_username(user,uname);
	compose_message(msg,MV_CONN_INIT,strtok(buff,"\n "),"");
	write_data(0,msg);
}

void ask_to_play(char * buff){
	
	memset(msg,0,MSG_SIZE);
	char * uname = strtok(buff,"\n ");
	compose_message(msg,MV_PLAY_WITH,get_username(user),uname);
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
		return 0;

	

	return movePiece(get_user_game(user),x1,x2,x3,x4);
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
		int move =validate_move(x1,x2,x3,x4); 
		if(move){
			
			memset(msg,0,MSG_SIZE);
			memset(payload,0,MSG_PAYLOAD_SIZE);
			sprintf(payload,"%c,%d,%c,%d",x1-10+'A',x2,x3-10+'A',x4);
			compose_message(msg,MV_MAKE_MOVE,get_username(user),payload);
			write_data(0,msg);
			get_board(get_user_game(user),board);
			
			if(move == 1)
				printBoard(board,"Oponents turn.");
			if(move == 2)
				printBoard(board,"Oponents turn. CHECK!");
			if(move == 3)
			{
				memset(msg,0,1024);
				compose_message(msg,MV_END_GAME,get_username(user),"");
				write_data(0,msg);
				won = 1;
				next_state = S_ENDG;
			}
			if(move == 4)
				printBoard(board,"Oponents turn. (pp)");//TODO : pawn promotion
		}
	}
	else return;
}

void handle_server_move(char *msg){
	memset(msg,0,MSG_SIZE);
	memset(payload,0,MSG_PAYLOAD_SIZE);
	decompose_message(msg,server,payload);
	char * start = payload;
		char * end;
		int x1 = strtoll(start,&end,20);
		start = end+1;
		int x2 = strtoll(start,&end,20);
		start = end+1;
		int x3 = strtoll(start,&end,20);
		start = end+1;
		int x4 = strtoll(start,&end,20);
		start = end+1;
		// wprintf(L"%d %d %d %d\n",x1,x2,x3,x4);
		int move = validate_move(x1,x2,x3,x4);
		if(move){
			get_board(get_user_game(user),board);
			if(move == 1)
				printBoard(board,"Your turn.");
			if(move == 2)
				printBoard(board,"Your turn. CHECK!");
			if(move == 3)
			{
				won = 0;
				next_state = S_ENDG;
			}
			if(move == 4)
				printBoard(board,"Your turn. (pp)");//TODO : pawn promotion
		}
}
void handle_server_data(char * msg){
}

void handle_forfeit(){
	won = 0;
	memset(msg,0,MSG_SIZE);
	compose_message(msg,MV_FORFEIT,get_username(user),"");
	write_data(0,msg);
}