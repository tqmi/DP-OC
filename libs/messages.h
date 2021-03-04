#if !defined(MESSAGES_H_INCLUDED)
#define MESSAGES_H_INCLUDED

#define MH_CONN_INIT 	"conn_init"
#define MH_AV_USERS 	"conn_init"
#define MH_PLAY_WITH 	"conn_init"
#define MH_GAME_REQ 	"game_request"
#define MH_MAKE_MOVE 	"make_move"
#define MH_CONN_END 	"conn_end"
#define MH_SET_COLOR 	"set_color"
#define MH_PLAYER_LEFT 	"player_left"
#define MH_ILLEGAL_MOVE "illegal_move"
#define SEP " "

#define MV_CONN_INIT	0
#define MV_AV_USERS		1
#define MV_PLAY_WITH 	2
#define MV_GAME_REQ 	3
#define MV_MAKE_MOVE 	4
#define MV_CONN_END 	5
#define MV_SET_COLOR 	6
#define MV_PLAYER_LEFT  7
#define MV_ILLEGAL_MOVE 8

int decompose_message(const char *message,char * user, char* payload);
int compose_message(char* msg, int msgType, const char* user,char* payload);

#endif // MESSAGES_H_INCLUDED
