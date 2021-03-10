#if !defined(UI_H_INCLUDED)
#define UI_H_INCLUDED

#define WHITE_KING 0x2654
#define WHITE_QUEEN 0x2655
#define WHITE_ROOK 0x2656
#define WHITE_BISHOP 0x2657
#define WHITE_KNITE 0x2658
#define WHITE_PAWN 0x2659

#define BLACK_KING 0x265A
#define BLACK_QUEEN 0x265B
#define BLACK_ROOK 0x265C
#define BLACK_BISHOP 0x265D
#define BLACK_KNITE 0x265E
#define BLACK_PAWN 0x265F

void init_UI(); //for character classification and conversion

void printMessage(char* message);// prints a message given as parameter

int giveAnswer(char answer); // return 0 or 1 depending on the giver parameter of type char ('n' or 'y')

void printBoard(int board[8][8],char *msg);//prints the 8x8 board along with the chess symbols

void printNameList(char *name_list);
#endif // UI_H_INCLUDED



