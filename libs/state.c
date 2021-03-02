#include "state.h"

int c_state;

void init_state(){
	c_state = S_INIT;
}

int get_state(){
	return c_state;
}

void set_state(int s){
	c_state = s;
}
