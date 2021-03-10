#if !defined(STATE_H_INCLUDED)
#define STATE_H_INCLUDED

#define S_INIT 0
#define S_AUTH 1
#define S_MENU 2
#define S_CONF 3
#define S_WAIT 4
#define S_PLAY 5
#define S_EXIT 6
#define S_ENDG 7

#define DELETED 0
#define ACTIVE  1
#define PLAYING 2


typedef struct user{
	char username [100];
	int fd;
	int state;
	struct game * game;
}t_user;

typedef struct game{
	struct user * white_player;
	struct user * black_player;
	int turn;
	int board[8][8];
	
	int whiteKingMoved; //used for castling
	int blackKingMoved;
}t_game;

t_game * init_state_game(t_user * white, t_user * black);
t_user * get_white_player(t_game* c_game);
t_user * get_black_player(t_game* c_game);
void get_board(t_game* c_game,int board[8][8]);
void set_turn(t_game * c_game, int t);
int get_turn(t_game* c_game);
void alter_player_turn(t_game * c_game);
void set_wkmoved(t_game * c_game, int v);
int get_wkmoved(t_game * c_game);
void set_bkmoved(t_game * c_game, int v);
int get_bkmoved(t_game * c_game);
int get_player_color(t_game * c_game,t_user * c_user);
t_user * init_state_user();
int get_state(t_user * c_user);
void set_state(t_user * c_user,int s);
void set_username(t_user * c_user,char * username);
char * get_username(t_user * c_user);
void set_user_fd(t_user * c_user,int fd);
int get_user_fd(t_user * c_user);
void set_user_game(t_user * c_user, t_game * c_game);
t_game * get_user_game(t_user * c_user);

#endif // STATE_H_INCLUDED
