#include <stdio.h>
#include <string.h>

void requestHandler(char request[], char *response, int fileDesc);
int processConnInit(char * user, char * payload, int fileDesc);
void processAvUsers(char * user, char * payload, int fileDesc,char * av_users);
int processPlayWith(char * user, char * payload, int fileDesc);
void create_game(int fd1, int fd2);
int processGameReq(char * user, char * payload, int fileDesc);

