#include <stdio.h>
#include <string.h>

const char SEP[2] = " ";

char * requestHandler(char request[])
{
    char *aux;  strcpy(aux, request);
    char *tok;
    char *action;
    char *file_descriptor;
    char params[128][30];

    tok = strtok(aux, SEP);

    strcpy(action, tok);

    tok = strtok(aux, SEP);

    strcpy(file_descriptor, tok);

    switch (actionHandler(action))
    {
        case 0:
            /* code */
            break;
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;
        case 8:
            /* code */
            break;
        default:
            break;
    }

    return request;
}

int actionHandler(char action[])
{
    if(strcmp(action, 'conn_init') == 0)    return 0;
    if(strcmp(action, 'av_users') == 0)     return 1;
    if(strcmp(action, 'play_with') == 0)    return 2;
    if(strcmp(action, 'game_request') == 0) return 3;
    if(strcmp(action, 'make_move') == 0)    return 4;
    if(strcmp(action, 'conn_end') == 0)     return 5;
    if(strcmp(action, 'set_color') == 0)    return 6;
    if(strcmp(action, 'player_left') == 0)  return 7;
    if(strcmp(action, 'illegal_move') == 0) return 8;

    return -1;
}