#if !defined(CHESS_LOGIC_H)
#define CHESS_LOGIC_H

#include <string.h>
#include <stdlib.h>
#include "state.h"

#define EMPTY 0

#define BPAWN -1
#define BKNIGHT -2 
#define BBISHOP -3
#define BROOK -4
#define BQUEEN -5
#define BKING -6

#define WPAWN 1
#define WKNIGHT 2
#define WBISHOP 3
#define WROOK 4
#define WQUEEN 5
#define WKING 6

#define BLACK -1
#define WHITE 1

//sets board
void init_board(int board[8][8]);



//returns 0 on invalid move
//returns 1 for valid move
//returns 2 for valid move with check
//returns 3 for valid move with mate
//returns 4 for valid move with pawn promotions
// x1,y1 - initial position, can have values between [1,8]
// x2,y2 - new position
int movePiece(t_game * c_game,int x1, int y1, int x2, int y2);

//promotes pawn to new piece
//use values from [2,5], will be adjusted automaticaly depending on player turn
//return 1 for valid piece value, 0 otherwise
int promotePawn(t_game * c_game,int newPiece);


#endif // CHESS_LOGIC_H