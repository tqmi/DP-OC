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


