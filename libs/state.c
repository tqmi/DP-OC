#include "state.h"
#include "../client/UI.h"
int c_state;

void init_state(){
	c_state = S_INIT;
}

int get_state(){
	return c_state;
}

void set_state(int s){

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
		printMessage("You are now playing!");
		break;
	case S_END:
		printMessage("Goodbye!");
		break;
	}
	c_state = s;
}
