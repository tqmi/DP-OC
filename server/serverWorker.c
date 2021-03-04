#include "../libs/messages.h"
#include <stdio.h>
#include <string.h>

const char SEP[2] = " ";

char * requestHandler(char request[], char *response)
{
    char *user = null;
    char *payload = null;

    switch (decompose_message(request, user, payload))
    {
        case 0:
            printf("%s%s", user, payload);
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
}