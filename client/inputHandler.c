#include "inputHandler.h"
#include "../libs/network.h"
#include <stdio.h>
#include <string.h>
#include "UI.h"
int handle_user_input(t_user * c_user, char * msg);
int handle_server_input(t_user * c_user, char * msg);
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

	return A_SER_DATA;
}


// int validate_move(char * buffer){

// 	int x1,x2,x3,x4;
// 	sscanf(buffer,"%c%d%c%d",&x1,&x2,&x3,&x4);
// 	x1 = 'a';
// 	x2 = 2;
// 	x3 = 'a';
// 	x4 = 4;
// 	if(x1 >= 'A' && x1 <= 'Z'){
// 		x1 = x1 - 'A' + 'a';
// 	}

// 	if(x3 >= 'A' && x3 <= 'Z'){
// 		x3 = x3 - 'A' + 'a';
// 	}

// 	if(x1 < 'a' || x1 > 'h' || x3 < 'a' || x3 > 'h' || x2 < 1 || x2 > 8 || x4 < 1 || x4 > 8)
// 		return -1;

// 	if(movePiece(get_user_game(user),x1,x2,x3,x4))
// 	    return 1;
//     return 0;
// }
