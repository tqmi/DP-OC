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
int N_USERS = 0;

// return an int from buffer
int get_int(char * buff){
	return strtol(buff,NULL,10);
}

// return a user pointer based on a file descriptor 
t_user * get_user_from_fd(int fd){
    for(int i = 0 ; i < N_USERS ; ++i){
        if(USERS[i]!=NULL && get_user_fd(USERS[i]) == fd ){
            return USERS[i];
        }
    }
    return NULL;
}

char msg[MSG_SIZE] = {0};
char user[MSG_ID_SIZE] = {0};
char payload[MSG_PAYLOAD_SIZE] = {0};
char ret_val[MSG_PAYLOAD_SIZE] = {0};

void requestHandler(char request[], int fileDesc, int connection)
{
    memset(msg,0,MSG_SIZE);
    memset(user,0,MSG_ID_SIZE);
    memset(payload,0,MSG_PAYLOAD_SIZE);

    int other_player;

    if(connection == 0)
    {

        t_user * discU = get_user_from_fd(fileDesc);
        if (discU == NULL) return;
        if(get_state(discU) == PLAYING){
            t_game * stopped_game = get_user_game(discU);
            t_user * otherU = NULL;

            if(get_player_color(stopped_game,discU) == WHITE)
                otherU = get_black_player(stopped_game);
            else
                otherU = get_white_player(stopped_game);
             
        
            compose_message(msg,MV_PLAYER_LEFT,SERVER_ID,get_username(discU));
            write_data(get_user_fd(otherU),msg);
            set_state(otherU,ACTIVE);
            free(stopped_game);

        }

        set_state(discU,DELETED);
        return;
    }

    switch (decompose_message(request, user, payload))
    {
        case MV_CONN_INIT:

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
                memset(payload,0,MSG_PAYLOAD_SIZE);
                strcat(payload,"2,");
                strcat(payload,user);
                compose_message(msg,MV_GAME_REQ,SERVER_ID,payload);
                write_data(get_user_fd(USERS[other_player]),msg);
                create_game(fileDesc,get_user_fd(USERS[other_player]));
            }else{
                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");
                write_data(fileDesc,msg);
            }
            break;
        case MV_GAME_REQ:
            if(processGameReq(user, payload,fileDesc)){
                set_turn(get_user_game(get_user_from_fd(fileDesc)),WHITE);
                compose_message(msg,MV_SET_COLOR,SERVER_ID,"1");
                write_data(get_user_fd(get_white_player(get_user_game(get_user_from_fd(fileDesc)))),msg);
                memset(msg,0,MSG_SIZE);
                compose_message(msg,MV_SET_COLOR,SERVER_ID,"-1");
                write_data(get_user_fd(get_black_player(get_user_game(get_user_from_fd(fileDesc)))),msg);
            }
            else{

                t_user * u = get_user_from_fd(fileDesc);
                if(get_state(u) != PLAYING || get_turn(get_user_game(u)) != 0)
                    return;
                
                
                t_game * game = get_user_game(u);
                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");

                set_state(get_white_player(game),ACTIVE);
                set_state(get_black_player(game),ACTIVE);


                write_data(get_user_fd(get_black_player(game)),msg);
                write_data(get_user_fd(get_white_player(game)),msg);

                free(game);
                
            }

            break;
        case MV_MAKE_MOVE:
            if(processMakeMove(user, payload,fileDesc)){
                compose_message(msg,MV_MAKE_MOVE,SERVER_ID,payload);
                t_user * u = get_user_from_fd(fileDesc);
                if(get_player_color(get_user_game(u),u) == WHITE)
                    write_data(get_user_fd(get_black_player(get_user_game(u))),msg);
                else
                    write_data(get_user_fd(get_white_player(get_user_game(u))),msg);
            }
            else{

            }
            break;
        case MV_FORFEIT:
            processForfeit(user, payload, fileDesc);
            break;
        case MV_END_GAME :
            processEndGame(user,payload,fileDesc); 
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

            if(strcmp(get_username(USERS[i]), user) == 0)
                return 0;
            
        }
    }

    if(indexFree == N_USERS)
    {
        USERS = (t_user **) realloc(USERS, (++N_USERS) * sizeof(t_user *));
    }
    else
    {
        free(USERS[indexFree]);
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
    memset(av_users,0,MSG_PAYLOAD_SIZE);
    for(int i = 0 ; i < N_USERS ; ++i)
    {
        if(get_state(USERS[i]) == ACTIVE && get_user_fd(USERS[i]) != fileDesc)
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
        if(get_state(USERS[i]) == ACTIVE && get_user_fd(USERS[i]) != fileDesc && strcmp(get_username(USERS[i]),payload) == 0)
        {
            return i;
        }
    }
    return -1;
}  

void create_game(int fd1, int fd2){
    t_user * u1 = NULL,*u2 = NULL;
    t_game * game;

    for(int i = 0 ; i < N_USERS ; ++i){
        if(get_user_fd(USERS[i]) == fd1 || get_user_fd(USERS[i]) == fd2){
            u1 = u2;
            u2 = USERS[i];
        }
    }
    set_state(u1,PLAYING);
    set_state(u2,PLAYING);
    game = init_state_game(u1,u2);
    set_turn(game,0);
}

int processGameReq(char * user, char * payload, int fileDesc){
    t_user *u = get_user_from_fd(fileDesc);
    
    if(get_state(u) != PLAYING || get_turn(get_user_game(u)) != 0)
        return 0;
    
    if(get_int(payload)){
        return 1;
    }
    else return 0;
} 

int processMakeMove(char * user, char * payload, int fileDesc) //TODO : verify player moves
{
    return 1;
}

void processForfeit(char * user, char * payload, int fileDesc)
{
    memset(msg,0,MSG_SIZE);

    t_user * u1 = get_user_from_fd(fileDesc);
    t_user * u2 = NULL;
    t_game * game = get_user_game(u1);
    int p_color = get_player_color(game,u1);
    if(p_color == WHITE)
        u2 = get_black_player(game);
    else
        u2 = get_white_player(game);
    
    compose_message(msg, MV_FORFEIT, SERVER_ID, get_username(u1));
    write_data(get_user_fd(u2), msg);
    set_state(u1,ACTIVE);
    set_state(u2,ACTIVE);
    free(game);
}

void processEndGame(char * user, char * payload, int fileDesc){
    t_user * u1 = get_user_from_fd(fileDesc);
    t_user * u2 = NULL;
    t_game * game = get_user_game(u1);
    int p_color = get_player_color(game,u1);
    if(p_color == WHITE)
        u2 = get_black_player(game);
    else
        u2 = get_white_player(game);
    set_state(u1,ACTIVE);
    set_state(u2,ACTIVE);
    free(game);
}