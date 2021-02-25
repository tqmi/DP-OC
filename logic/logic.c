#include <stdio.h>
#include <string.h>
#include "logic.h"

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


int board[8][8];


void setStartingBoard()
{
    //Pawns
    for(int i=0;i<8;i++) 
    {
        board[1][i]=bP;
        board[6][i]=wP;
    }

    //Knights
    board[0][1]=board[0][6]=bN;
    board[7][1]=board[7][6]=wN;

    //Bishops
    board[0][2]=board[0][5]=bB;
    board[7][2]=board[7][5]=wB;

    //Rooks
    board[0][0]=board[0][7]=bR;
    board[7][0]=board[7][7]=wR;

    //Queens
    board[0][4]=bQ;
    board[7][4]=wQ;

    //Kings
    board[0][3]=bK;
    board[7][3]=wK;


}

//copies the board into the board given as a parameter
void getBoard(int auxBoard[8][8])
{

    memcpy(auxBoard,board, 8*8*sizeof(int));

}











//for testing
void printBoard(int board[8][8])
{
    for(int i =0;i<8;i++)
    {
        for(int j=0;j<8;j++) {
            
            if(board[i][j]< 0) printf(" %d",board[i][j]);
            else printf("  %d",board[i][j]);
        }
        printf("\n");
    }
}



