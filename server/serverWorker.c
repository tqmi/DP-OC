#include "../libs/messages.h"
#include <stdio.h>
#include <string.h>

void requestHandler(char request[], char *response)
{
    char user[128] = {0};
    char payload[1024] = {0};

    switch (decompose_message(request, user, payload))
    {
        case MV_CONN_INIT:
            printf("%s%s", user, payload);
            break;
        case MV_AV_USERS:
            /* code */
            break;
        case MV_PLAY_WITH:
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