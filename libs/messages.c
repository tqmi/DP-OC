#include "messages.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char aux[MSG_SIZE];
char type[MSG_TYPE_SIZE];
//returns the numeric code of the message type or -1 otherwise
int get_msg_type_from_name(const char * action)
{
    if(strcmp(action, MH_CONN_INIT) == 0)    return MV_CONN_INIT;
    if(strcmp(action, MH_AV_USERS) == 0)     return MV_AV_USERS;
    if(strcmp(action, MH_PLAY_WITH) == 0)    return MV_PLAY_WITH;
    if(strcmp(action, MH_GAME_REQ) == 0)     return MV_GAME_REQ;
    if(strcmp(action, MH_MAKE_MOVE) == 0)    return MV_MAKE_MOVE;
    if(strcmp(action, MH_FORFEIT) == 0)      return MV_FORFEIT;
    if(strcmp(action, MH_SET_COLOR) == 0)    return MV_SET_COLOR;
    if(strcmp(action, MH_PLAYER_LEFT) == 0)  return MV_PLAYER_LEFT;
    if(strcmp(action, MH_ILLEGAL_MOVE) == 0) return MV_ILLEGAL_MOVE;
    if(strcmp(action, MH_END_GAME) == 0)     return MV_END_GAME;

    return -1;
}

int get_msg_type_from_code(int code,char * type){
	if(code == MV_CONN_INIT)    {strcpy(type,MH_CONN_INIT);return 1;}
    if(code == MV_AV_USERS)     {strcpy(type,MH_AV_USERS);return 1;}
    if(code == MV_PLAY_WITH)    {strcpy(type,MH_PLAY_WITH);return 1;}
    if(code == MV_GAME_REQ)     {strcpy(type,MH_GAME_REQ);return 1;}
    if(code == MV_MAKE_MOVE)    {strcpy(type,MH_MAKE_MOVE);return 1;}
    if(code == MV_FORFEIT)      {strcpy(type,MH_FORFEIT);return 1;}
    if(code == MV_SET_COLOR)    {strcpy(type,MH_SET_COLOR);return 1;}
    if(code == MV_PLAYER_LEFT)  {strcpy(type,MH_PLAYER_LEFT);return 1;}
    if(code == MV_ILLEGAL_MOVE) {strcpy(type,MH_ILLEGAL_MOVE);return 1;}
    if(code == MV_END_GAME)     {strcpy(type,MH_END_GAME);return 1;}
	return -1;
}

//returns the originating user,payload and message type(numeric code) or -1 otherwise
int decompose_message(const char* message,char* user, char* payload){
    memset(aux,0,MSG_SIZE);
    memset(type,0,MSG_TYPE_SIZE);
    strcpy(aux, message);
    char *tok;
    tok = strtok(aux, SEP);

    if(tok != NULL) {
	    strcpy(type, tok);
    }

    tok = strtok(NULL, SEP);

    if(tok != NULL) {
	    strcpy(user, tok);
    }

	tok = strtok(NULL, SEP);

    if(tok != NULL) {
	    strcpy(payload,tok);
    }

	return get_msg_type_from_name(type);
}

int compose_message(char* msg, int msgType, const char* user,char* payload){
	memset(type,0,MSG_TYPE_SIZE);
	if(get_msg_type_from_code(msgType,type) >=0 ){
		strcpy(msg,type);
		strcat(msg,SEP);
		strcat(msg,user);
		strcat(msg,SEP);
		strcat(msg,payload);
		strcat(msg,"\0");
	}
	return -1;
}

