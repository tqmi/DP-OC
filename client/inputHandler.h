#if !defined(INPUTHANDLER_H_INCLUDED)
#define INPUTHANDLER_H_INCLUDED
#include "../libs/state.h"

#define A_NO_ACTION	0
#define A_REFRESH   1
#define A_INCORRECT	2
#define A_USER_DATA	3
#define A_CONF_REQ  4
#define A_QUIT      5
#define A_FORFEIT   6
#define A_SER_DATA  7
#define A_ERROR     8
#define A_SER_DISC  9
#define A_CORRECT   10
#define A_ST_GAME   11
#define A_OP_LEFT   12
#define A_LIST_USERS 13

int handle_inputs(t_user * c_user,char* buffer,int buff_size);



#endif // INPUTHANDLER_H_INCLUDED
