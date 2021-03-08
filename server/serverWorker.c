#include "../libs/messages.h"
#include "../libs/state.h"
#include "../libs/network.h"
#include "./serverWorker.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_ID "server"

t_user **USERS;
t_game **GAMES;
int N_USERS;

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
                compose_message(msg,MV_GAME_REQ,SERVER_ID,"2");
                write_data(get_user_fd(USERS[other_player]),msg);
            }else{
                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");
                write_data(fileDesc,msg);
            }

            /* code */
            break;
        case MV_GAME_REQ:
            
            /* code */
            break;
        case MV_MAKE_MOVE:
            /* code */
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