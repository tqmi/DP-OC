#if !defined(STATE_H_INCLUDED)
#define STATE_H_INCLUDED

#define S_INIT 0
#define S_AUTH 1
#define S_MENU 2
#define S_CONF 3
#define S_WAIT 4
#define S_PLAY 5
#define S_END 6


void init_state();
int get_state();
void set_state(int s);

#endif // STATE_H_INCLUDED
