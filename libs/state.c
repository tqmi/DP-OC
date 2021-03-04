#include "state.h"
#include "../client/UI.h"
#include "logic.h"
#include <stdlib.h>
#include <string.h>

t_game * init_state_game(t_user * white, t_user * black){
	t_game * c_game = (t_game *)malloc(sizeof(t_game));
	c_game->white_player = white;
	c_game->black_player = black;
	set_user_game(white,c_game);
	set_user_game(black,c_game);
}

void get_board(t_game* c_game,int board[8][8]){
	memcpy(board,c_game->board, 8 * 8 * sizeof(int));
}

t_user * init_state_user(){
	t_user * c_user = (t_user *)malloc(sizeof(t_user));
	c_user->state = S_INIT;
	return c_user;
}

int get_state(t_user * c_user){
	return c_user->state;
}

void set_state(t_user * c_user,int s)
{
	switch (s)
	{
	case S_INIT:
		break;
	case S_AUTH:
		printMessage("Enter your username :");
		break;
	case S_MENU:
		printMessage("You are in the menu!\nType exit to exit!");
		break;
	case S_WAIT:
		printMessage("Please wait for your opponent to accept invitation!");
		break;
	case S_CONF:
		printMessage("Do you want to play with ... ? (yes/no)");
		break;
	case S_PLAY:
		// printMessage("You are now playing! You are white!");
		// getBoard(board);
		// printBoard(board);
		// player_color = 1;
		break;
	case S_END:
		printMessage("Goodbye!");
		break;
	}
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