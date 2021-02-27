#if !defined(CHESS_LOGIC_H)
#define CHESS_LOGIC_H

#include <string.h>
#include <stdlib.h>

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


void setStartingBoard();

//copies the board into the board given as a parameter
void getBoard(int auxBoard[8][8]);

//returns 0 on invalid move 
//returns 1 for valid move
//returns 2 for valid move with check
//returns 3 for valid move with mate
// x1,y1 - initial position, can have values between [1,8]
// x2,y2 - new position
int movePiece(int x1, int y1, int x2, int y2);

//return 1 if its white's turn and -1 for black
int getPlayerTurn();

//promotes pawn to new piece
//use values from [2,5], will be adjusted automaticaly depending on player turn
//return 1 for valid piece value, 0 otherwise
int promotePawn(int newPiece);

//a basic print of the board
//for testing, should be removed in the future
void printBoard(int board[8][8]);


#endif // CHESS_LOGIC_H