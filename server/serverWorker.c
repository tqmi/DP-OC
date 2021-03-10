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
int N_USERS = 0;
int N_GAMES = 0;

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


void requestHandler(char request[], char *response, int fileDesc, int connection)
{
    char user[128] = {0};
    char payload[1024] = {0};
    char msg[1024] = {0};
    char ret_val[1024] = {0};
    int other_player;

    if(connection == 0)
    {
        t_game * stopped_game = NULL;
        
        for(int i = 0 ; i < N_USERS ; ++i)
        {
           if(get_user_fd(USERS[i]) == fileDesc)
           {
               stopped_game = get_user_game(USERS[i]);
               set_state(USERS[i], DELETED);
               
               if(get_user_fd(get_black_player(stopped_game)) == fileDesc)
               {
                   set_state(get_user_from_fd(get_user_fd(get_white_player(stopped_game))), ACTIVE);
               }
               else
               {
                   set_state(get_user_from_fd(get_user_fd(get_black_player(stopped_game))), ACTIVE);
               }
           }
        }
        
        int blk_fd = get_user_fd(get_black_player(stopped_game));
        int wht_fd = get_user_fd(get_white_player(stopped_game));
        char * blk_username = get_username(get_black_player(stopped_game));
        char * wht_username = get_username(get_white_player(stopped_game));

        free(stopped_game);
        
        printf("%s %s\n",wht_username,blk_username);
        if(wht_fd == fileDesc)
        {
            // white player disconnected
            compose_message(msg, MV_PLAYER_LEFT, SERVER_ID, wht_username);
            write_data(blk_fd, msg);
        }
        else if(blk_fd == fileDesc)
        {
            // black player disconnected
            compose_message(msg, MV_PLAYER_LEFT, SERVER_ID, blk_username);
            write_data(wht_fd, msg);
        }

        for (int  i = 0 ; i < N_GAMES ; ++i)
        {
            if((get_state(get_white_player(GAMES[i])) == DELETED) || (get_state(get_black_player(GAMES[i])) == DELETED))
            {
                free(GAMES[i]);
                for(int j = i ; j < (N_GAMES - 1) ; ++j)
                {
                    GAMES[j] = GAMES[j + 1];
                }

                N_GAMES--;
                break;
            }
        }
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
                memset(payload,0,1024);
                strcat(payload,"2,");
                strcat(payload,user);
                compose_message(msg,MV_GAME_REQ,SERVER_ID,payload);
                // printf("%s\n",msg);
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
                write_data(get_user_fd(get_white_player(get_user_game(get_user_from_fd(fileDesc)))),msg);
                memset(msg,0,1024);
                compose_message(msg,MV_SET_COLOR,SERVER_ID,"-1");
                write_data(get_user_fd(get_black_player(get_user_game(get_user_from_fd(fileDesc)))),msg);
            }
            else{

                t_user * u = get_user_from_fd(fileDesc);
                if(get_turn(get_user_game(u)) != 0)
                    return;

                compose_message(msg,MV_GAME_REQ,SERVER_ID,"0");
                
                write_data(get_user_fd(get_black_player(get_user_game(u))),msg);
                write_data(get_user_fd(get_white_player(get_user_game(u))),msg);

                // free(get_user_game(u));
                //TODO : free game space
                
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
                // printf("%s\n",msg);
            }
            else{

            }
            break;
        case MV_FORFEIT:
            processForfeit(user, payload, fileDesc);
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

    for(int i = 0 ; i < N_USERS ; ++i){
        if(get_user_fd(USERS[i]) == fd1 || get_user_fd(USERS[i]) == fd2){
            u1 = u2;
            u2 = USERS[i];
            set_state(USERS[i], PLAYING);
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

int processMakeMove(char * user, char * payload, int fileDesc) //TODO : verify player moves
{
    return 1;
}

void processForfeit(char * user, char * payload, int fileDesc)
{
    char msg[1024] = {0};

    t_user * u = get_user_from_fd(fileDesc);
    t_game * game = get_user_game(u);
    int p_color = get_player_color(game,u);
    if(p_color == WHITE){
        compose_message(msg, MV_FORFEIT, SERVER_ID, get_username(u));
        write_data(get_user_fd(get_black_player(game)), msg);
    }else{
        compose_message(msg, MV_FORFEIT, SERVER_ID, get_username(u));
        write_data(get_user_fd(get_white_player(game)), msg);
    }



    // for(int i = 0 ; i < N_GAMES ; ++i)
    // {   
    //     if(get_user_fd(get_white_player(GAMES[i])) == fileDesc)
    //     {
    //         // white player forfeited
    //         compose_message(msg, MV_FORFEIT, SERVER_ID, get_username(get_white_player(GAMES[i])));
    //         write_data(get_user_fd(get_black_player(GAMES[i])), msg);
    //     }
    //     else if(get_user_fd(get_black_player(GAMES[i])) == fileDesc)
    //     {
    //         // black player forfeited
    //         compose_message(msg, MV_FORFEIT, SERVER_ID, get_username(get_black_player(GAMES[i])));
    //         write_data(get_user_fd(get_white_player(GAMES[i])), msg);
    //     }
    // }
}