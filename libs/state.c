#include "state.h"
#include "../client/UI.h"
#include "logic.h"
#include <stdlib.h>
#include <string.h>

t_game * init_state_game(t_user * white_p, t_user * black_p){
	t_game * c_game = (t_game *)malloc(sizeof(t_game));
	if(white_p != NULL){
		c_game->white_player = white_p;
		set_user_game(white_p,c_game);
	}
	if(black_p != NULL){
		c_game->black_player = black_p;
		set_user_game(black_p,c_game);
	}
	init_board(c_game->board);
	set_turn(c_game,WHITE);
	set_wkmoved(c_game,0);
	set_bkmoved(c_game,0);
	return c_game;
}

t_user * get_white_player(t_game* c_game)
{
	return c_game->white_player;
}

t_user * get_black_player(t_game* c_game)
{
	return c_game->black_player;
}

void get_board(t_game* c_game,int board[8][8]){
	memcpy(board,c_game->board, 8 * 8 * sizeof(int));
}

void set_turn(t_game * c_game, int t){
	c_game->turn = t;
}
int get_turn(t_game* c_game){
	return c_game->turn;
}
void alter_player_turn(t_game * c_game){
	c_game->turn *=-1; 
}

void set_wkmoved(t_game * c_game, int v){
	c_game->whiteKingMoved = v;
}

int get_wkmoved(t_game * c_game){
	return c_game->whiteKingMoved;
}

void set_bkmoved(t_game * c_game, int v){
	c_game->blackKingMoved = v;
}


int get_bkmoved(t_game * c_game){
	return c_game->blackKingMoved;
}


int get_player_color(t_game * c_game,t_user * c_user){
	if(c_game->white_player == c_user) return WHITE;
	if(c_game->black_player == c_user) return BLACK;
	return 0;
}

t_user * init_state_user(){
	t_user * c_user = (t_user *)malloc(sizeof(t_user));
	c_user->state = S_INIT;
	c_user->game = NULL;
	return c_user;
}

int get_state(t_user * c_user){
	return c_user->state;
}

void set_state(t_user * c_user,int s)
{
	// switch (s)
	// {
	// case S_INIT:
	// 	break;
	// case S_AUTH:
	// 	printMessage("Enter your username :");
	// 	break;
	// case S_MENU:
	// 	printMessage("You are in the menu!\nType exit to exit!");
	// 	break;
	// case S_WAIT:
	// 	printMessage("Please wait for your opponent to accept invitation!");
	// 	break;
	// case S_CONF:
	// 	printMessage("Do you want to play with ... ? (yes/no)");
	// 	break;
	// case S_PLAY:
	// 	init_state_game(c_user,NULL);
	// 	int board[8][8];
	// 	get_board(get_user_game(c_user),board);
	// 	printBoard(board);
	// 	break;
	// case S_EXIT:
	// 	printMessage("Goodbye!");
	// 	break;
	// }
	c_user -> state = s;
}

void set_username(t_user * c_user,char * username){
	strncpy(c_user->username,username,100);
}

char * get_username(t_user * c_user){
	return c_user->username;
}

void set_user_fd(t_user * c_user,int fd){
	c_user->fd = fd;
}

int get_user_fd(t_user * c_user){
	return c_user->fd;
}

void set_user_game(t_user * c_user, t_game * c_game){
	c_user->game = c_game;
}

t_game * get_user_game(t_user * c_user){
	return c_user->game;
}