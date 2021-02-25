#if !defined(CHESS_LOGIC_H)
#define CHESS_LOGIC_H

#define empty 0

#define bP -1
#define bN -2 //knight, in chess is noted with N
#define bB -3
#define bR -4
#define bQ -5
#define bK -6

#define wP  1
#define wN  2 
#define wB  3
#define wR  4
#define wQ  5
#define wK  6


void setStartingBoard();

//copies the board into the board given as a parameter
void getBoard(int auxBoard[8][8]);

//a basic print of the board
//for testing, should be removed in the future
void printBoard(int board[8][8]);

#endif // CHESS_LOGIC_H