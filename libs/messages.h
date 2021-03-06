#if !defined(MESSAGES_H_INCLUDED)
#define MESSAGES_H_INCLUDED

#define MH_CONN_INIT 	"conn_init"
#define MH_AV_USERS 	"av_users"
#define MH_PLAY_WITH 	"play_with"
#define MH_GAME_REQ 	"game_request"
#define MH_MAKE_MOVE 	"make_move"
#define MH_FORFEIT 	    "forfeit"
#define MH_SET_COLOR 	"set_color"
#define MH_PLAYER_LEFT 	"player_left"
#define MH_ILLEGAL_MOVE "illegal_move"
#define MH_END_GAME		"end_game"
#define SEP " "

#define MV_CONN_INIT    0
#define MV_AV_USERS     1
#define MV_PLAY_WITH    2
#define MV_GAME_REQ     3
#define MV_MAKE_MOVE    4
#define MV_FORFEIT      5
#define MV_SET_COLOR    6
#define MV_PLAYER_LEFT  7
#define MV_ILLEGAL_MOVE 8
#define MV_END_GAME		9

#define MSG_SIZE 1024
#define MSG_TYPE_SIZE 32
#define MSG_ID_SIZE 128
#define MSG_PAYLOAD_SIZE 862 // its 2 less because there are 2 separator spaces

int decompose_message(const char *message,char * user, char* payload);
int compose_message(char* msg, int msgType, const char* user,char* payload);

#endif // MESSAGES_H_INCLUDED
