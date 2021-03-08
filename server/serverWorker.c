#include "../libs/messages.h"
#include "../libs/state.h"
#include "../libs/network.h"
#include "./serverWorker.h"
#include "../libs/logic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_ID "server"

t_user **USERS;
t_game **GAMES;
int N_USERS;
int N_GAMES;

int get_int(char * buff){
	return strtol(buff,NULL,10);
}

t_user * get_user_from_fd(int fd){
    for(int i = 0 ; i < N_USERS ; ++i){
        if(USERS[i]!=NULL && get_user_fd(USERS[i]) == fd ){
            return USERS[i];
        }
    }
    return NULL;
}


void requestHandler(char request[], char *response, int fileDesc)
{
    char user[128] = {0};
    char payload[1024] = {0};
    char msg[1024] = {0};
    char ret_val[1024] = {0};
    int other_player;
    switch (decompose_message(request, user, payload))
    {
        case MV_CONN_INIT:
            // printf("%s%s", user, payload);

            if(processConnInit(user, payload, fileDesc))
            {
                // username available, user initiated 
                compose_message(msg, MV_CONN_INIT, SERVER_ID, "1");
            }
            else
            {
                // username unavailable
                compose_message(msg, MV_CONN_INIT, SERVER_ID, "0");
            }

            write_data(fileDesc, msg);

            break;
        case MV_AV_USERS:
            processAvUsers(user,payload,fileDesc,ret_val);
            compose_message(msg,MV_AV_USERS,SERVER_ID,ret_val);
            write_data(fileDesc,msg);
            break;
        case MV_PLAY_WITH:
            
            if((other_player = processPlayWith(user,payload,fileDesc)) >= 0) //returns -1 if not ok and >= 0 if ok
            {
                memset(payload,0,1024);
                strcat(payload,"2,");
                strcat(payload,user);
                compose_message(msg,MV_GAME_REQ,SERVER_ID,payload);
                printf("%s\n",msg);
                write_data(get_user_fd(USERS[other_player]),msg);
                create_game(fileDesc,get_user_fd(USERS[other_player]));
            }else{
                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");
                write_data(fileDesc,msg);
            }

            /* code */
            break;
        case MV_GAME_REQ:
            if(processGameReq(user, payload,fileDesc)){
                set_turn(get_user_game(get_user_from_fd(fileDesc)),WHITE);
                compose_message(msg,MV_SET_COLOR,SERVER_ID,"1");
                write_data(get_user_fd(get_white_payer(get_user_game(get_user_from_fd(fileDesc)))),msg);
                memset(msg,0,1024);
                compose_message(msg,MV_SET_COLOR,SERVER_ID,"-1");
                write_data(get_user_fd(get_black_payer(get_user_game(get_user_from_fd(fileDesc)))),msg);
            }
            else{

                t_user * u = get_user_from_fd(fileDesc);
                if(get_turn(get_user_game(u)) != 0)
                    return;

                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");
                
                write_data(get_user_fd(get_black_payer(get_user_game(u))),msg);
                write_data(get_user_fd(get_white_payer(get_user_game(u))),msg);

                // free(get_user_game(u));
                //TODO : free game space
                
            }

            break;
        case MV_MAKE_MOVE:
            if(processMakeMove(user, payload,fileDesc)){

            }
            else{

            }
            break;
        case MV_CONN_END:
            /* code */
            break;
        case MV_SET_COLOR:
            /* code */
            break;
        case MV_PLAYER_LEFT:
            /* code */
            break;
        case MV_ILLEGAL_MOVE:
            /* code */
            break;
        default:
            break;
    }
}

int processConnInit(char * user, char * payload, int fileDesc)
{
    int indexFree = N_USERS;

    for(int i = 0 ; i < N_USERS ; ++i)
    {
        if(get_state(USERS[i]) == DELETED)
        {
            indexFree = i;
        }
        else
        {
            // printf("%s %s", get_username(USERS[i]), user);
            if(strcmp(get_username(USERS[i]), user) == 0)
            {
                // printf("in if\n");
                return 0;
            }
        }
    }

    if(indexFree == N_USERS)
    {
        USERS = (t_user **) realloc(USERS, (++N_USERS) * sizeof(t_user *));
    }

    USERS[indexFree] = init_state_user();

    set_username(USERS[indexFree], user);
    set_user_fd(USERS[indexFree], fileDesc);
    set_state(USERS[indexFree], ACTIVE);
    printf("%d %s connected\n",fileDesc,user);
    return 1;
}

void processAvUsers(char * user, char * payload, int fileDesc, char * av_users)
{

    for(int i = 0 ; i < N_USERS ; ++i)
    {
        if(get_state(USERS[i]) != DELETED && get_user_fd(USERS[i]) != fileDesc)
        {
            strcat(av_users,get_username(USERS[i]));
            strcat(av_users,",\0");
        }
    }

}

int processPlayWith(char * user, char * payload, int fileDesc) // returns the index of the opponent or -1 if not found
{
    for(int i = 0 ; i < N_USERS ; ++i)
    {
        if(get_state(USERS[i]) != DELETED && get_user_fd(USERS[i]) != fileDesc && strcmp(get_username(USERS[i]),payload) == 0)
        {
            return i;
        }
    }
    return -1;
}  

void create_game(int fd1, int fd2){
    t_user * u1 = NULL,*u2 = NULL;

    for(int i = 0 ; i < N_USERS ; ++i){
        if(get_user_fd(USERS[i]) == fd1 || get_user_fd(USERS[i]) == fd2){
            u1 = u2;
            u2 = USERS[i];
        }
    }
    int free_slot = N_GAMES;
    for(int i = 0 ; i < N_GAMES ; ++i){
        if(GAMES[i] == NULL){
            free_slot = i;
        }
    }
    if(free_slot == N_GAMES){
        GAMES = (t_game **) realloc(GAMES, (++N_GAMES) * sizeof(t_game *));
    }

    GAMES[free_slot] = init_state_game(u1,u2);
    set_turn(GAMES[free_slot],0);

}

int processGameReq(char * user, char * payload, int fileDesc){
    t_user *u = get_user_from_fd(fileDesc);
    if(get_turn(get_user_game(u)) != 0)
        return 0;
    
    if(get_int(payload)){
        return 1;
    }
    else return 0;
} 