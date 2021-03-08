#include <stdio.h>
#include <string.h>

void requestHandler(char request[], char *response, int fileDesc);
int processConnInit(char * user, char * payload, int fileDesc);
