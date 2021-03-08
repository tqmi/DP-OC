#include "inputHandler.h"
#include "../libs/network.h"
#include "../libs/messages.h"
#include "../libs/logic.h"
#include <stdio.h>
#include <string.h>
#include "UI.h"
#include <stdlib.h>

int handle_user_input(t_user * c_user, char * msg);
int handle_server_input(t_user * c_user, char * msg);

int get_int(char * buff){
	return strtol(buff,NULL,10);
}

int handle_inputs(t_user * c_user,char* buffer,int buff_size){
	int bytes_read;
	int data_source;
	bytes_read = read_data(buffer,buff_size,&data_source);

	if(bytes_read > 0){

		if(data_source == KEYBOARDIN){
			//KEYBOARD
			return handle_user_input(c_user,buffer);
		}
		else{
			//Server
			return handle_server_input(c_user,buffer);
		}
	}
	else if (bytes_read == 0 && data_source >= 0)
	{
		return A_SER_DISC;
	}
	else if (bytes_read == 0 && data_source == -1)
	{
		return A_NO_ACTION;
	}
	else
	{
		return A_ERROR;
	}
	return A_NO_ACTION;
}

int handle_user_input(t_user * c_user, char * msg){
	if(strcmp(msg,"refresh\n") == 0){
		return A_REFRESH;
	}
	else if (strcmp(msg,"exit\n") == 0){
		return A_QUIT;
	}
	return A_USER_DATA;
}

int handle_server_input(t_user * c_user, char * msg){

	int msg_type;
	char server[100];memset(server,0,100);
	char payload[1024];memset(payload,0,1024);
	msg_type = decompose_message(msg,server,payload);
	// char users[100][100];
	if(strcmp(server,"server")!=0)
		return A_NO_ACTION;
	
	switch(msg_type){
	case MV_CONN_INIT   :
		if(get_int(payload)) {
			return A_CORRECT;
		}else {
			return A_INCORRECT;
		}
	break;
	case MV_AV_USERS    : //TODO
		if(get_state(c_user) == S_MENU){
			// get_user_list(payload,users);
			// next_state = S_MENU;
		}
	break;
	case MV_GAME_REQ    :
		if(get_int(payload) == 2){
			return A_CONF_REQ;
		}
		else if(get_int(payload) == 0){
			return A_INCORRECT;
		}
		else if(get_int(payload) == 1){
				// wait for color
		}
	break;
	case MV_MAKE_MOVE   : // TODO
		if(get_state(c_user) == S_PLAY){
			// get_board(get_user_game(c_user),board);
			// movePiece(board); 
		}
	break;
	case MV_SET_COLOR   :
		if(get_int(payload) == WHITE){
			init_state_game(c_user,NULL);
		}else{
			init_state_game(NULL,c_user);
		}
		return A_ST_GAME;
	break;
	case MV_PLAYER_LEFT : 
		return A_OP_LEFT;
	break;
	case MV_ILLEGAL_MOVE: 
		if(get_state(c_user) == S_PLAY){
			// TODO illegal move
		}
	break;
	}


	return A_SER_DATA;
}


