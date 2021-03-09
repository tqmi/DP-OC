#include <stdio.h>
#include <string.h>

void requestHandler(char request[], char *response, int fileDesc, int connection);
void create_game(int fd1, int fd2);
void processAvUsers(char * user, char * payload, int fileDesc,char * av_users);
// void processForfeit(char * user, char * payload, int fileDesc);
int processConnInit(char * user, char * payload, int fileDesc);
int processPlayWith(char * user, char * payload, int fileDesc);
int processGameReq(char * user, char * payload, int fileDesc);
int processMakeMove(char * user, char * payload, int fileDesc);
