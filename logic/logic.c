#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logic.h"

#define empty 0

#define bP -1
#define bN -2 //knight, in chess is noted with N
#define bB -3
#define bR -4
#define bQ -5
#define bK -6

#define wP 1
#define wN 2
#define wB 3
#define wR 4
#define wQ 5
#define wK 6

void resizeCoord(int *x1, int *y1, int *x2, int *y2);
int basicCheck(int x1, int y1, int x2, int y2);
int advancedCheck(int x1, int y1, int x2, int y2);
int pawnCheck(int x1, int y1, int x2, int y2);
int blackPawnCheck(int x1, int y1, int x2, int y2);
int whitePawnCheck(int x1, int y1, int x2, int y2);
int knightCheck(int x1, int y1, int x2, int y2);
int bishopCheck(int x1, int y1, int x2, int y2);
int rookCheck(int x1, int y1, int x2, int y2);
int queenCheck(int x1, int y1, int x2, int y2);
int kingCheck(int x1, int y1, int x2, int y2);

int board[8][8];

void setStartingBoard()
{
    //Pawns
    for (int i = 0; i < 8; i++)
    {
        board[i][1] = wP;
        board[i][6] = bP;
    }

    //Knights
    board[1][0] = board[6][0] = wN;
    board[1][7] = board[6][7] = bN;

    //Bishops
    board[2][0] = board[5][0] = wB;
    board[2][7] = board[5][7] = bB;

    //Rooks
    board[0][0] = board[7][0] = wR;
    board[0][7] = board[7][7] = bR;

    //Queens
    board[4][0] = wQ;
    board[4][7] = bQ;

    //Kings
    board[3][0] = wK;
    board[3][7] = bK;
}

//copies the board into the board given as a parameter
void getBoard(int auxBoard[8][8])
{
    memcpy(auxBoard, board, 8 * 8 * sizeof(int));
}

//for testing
void printBoard(int board[8][8])
{
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {

            if (board[j][i] < 0)
                printf(" %d", board[j][i]);
            else
                printf("  %d", board[j][i]);
        }

        printf(" |%d\n", i + 1);
    }
    for (int j = 0; j < 8; j++)
        printf("  _");
    printf("\n");
    for (int j = 0; j < 8; j++)
        printf("  %d", j + 1);
    printf("\n");
}

//returns 0 on invalid move, otherwise returns 1
// x1,y1 - initial position, can have values between [1,8]
// x2,y2 - new position
int movePiece(int x1, int y1, int x2, int y2)
{
    resizeCoord(&x1, &y1, &x2, &y2);
    if (!basicCheck(x1, y1, x2, y2))
        return 0;

    if (!advancedCheck(x1, y1, x2, y2))
        return 0;

    int pieceType = board[x1][y1];
    board[x2][y2] = pieceType;
    board[x1][y1] = 0;

    return 1;
}

//makes the coord from user frendly to array frendly
void resizeCoord(int *x1, int *y1, int *x2, int *y2)
{
    *x1 -= 1;
    *y1 -= 1;
    *x2 -= 1;
    *y2 -= 1;
}

//checks general things that are valid for all pieces
// return 0 on incorrect imput, 1 otherwise
int basicCheck(int x1, int y1, int x2, int y2)
{
    //not outside of board check
    if (x1 > 7 || y1 > 7 || x2 > 7 || y2 > 7)
        return 0;
    if (x1 < 0 || x1 < 0 || x1 < 0 || x1 < 0)
        return 0;

    //non empty start check
    if (board[x1][y1] == 0)
        return 0;

    //not ending on same color piece check
    if (board[x2][y2] != 0)
    {
        int pieceColorStart = (board[x1][y1] > 0) ? 1 : -1; //1 white, -1 black
        int pieceColorEnd = (board[x2][y2] > 0) ? 1 : -1;

        if (pieceColorStart == pieceColorEnd)
            return 0;
    }

    return 1;
}

//takes into account :
//-the type of the piece
//-whether its path is blocked
int advancedCheck(int x1, int y1, int x2, int y2)
{
    int pieceType = board[x1][y1];

    switch (pieceType)
    {
    case -1:
    case 1:
        if (!pawnCheck(x1, y1, x2, y2))
            return 0;
        break;

    case -2:
    case 2:
        if (!knightCheck(x1, y1, x2, y2))
            return 0;
        break;

    case -3:
    case 3:
        if (!bishopCheck(x1, y1, x2, y2))
            return 0;
        break;

    case -4:
    case 4:
        if (!rookCheck(x1, y1, x2, y2))
            return 0;
        break;

    case -5:
    case 5:
        if (!queenCheck(x1, y1, x2, y2))
            return 0;
        break;

    case -6:
    case 6:
        if (!kingCheck(x1, y1, x2, y2))
            return 0;
        break;

    default:
        return 0;
    }

    return 1;
}

int pawnCheck(int x1, int y1, int x2, int y2)
{
    int pawnColor = board[x1][y1];

    if (pawnColor > 0)
        return whitePawnCheck(x1, y1, x2, y2);
    else
        return blackPawnCheck(x1, y1, x2, y2);
}

int whitePawnCheck(int x1, int y1, int x2, int y2)
{

    int xDiff = x1 - x2;
    int yDiff = y1 - y2;

    if (yDiff == -2 && xDiff == 0 && y1 == 1) //moving 2 sqares
    {
        if (board[x1][y1 + 1] == 0 && board[x1][y1 + 2] == 0)
            return 1;
        else
            return 0;
    }

    if (abs(xDiff) == 1 && yDiff == -1) //taking a piece
    {
        if (board[x2][y2] < 0)
            return 1;
        else
            return 0;
    }

    if (yDiff == -1 && board[x1][y1 + 1] == 0)
        return 1;
    else
        return 0;

    if (abs(xDiff) > 0)
        return 0;
    if (yDiff < -1)
        return 0;
    if (yDiff > 0)
        return 0;

    return 1;
}

int blackPawnCheck(int x1, int y1, int x2, int y2)
{
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;

    if (yDiff == 2 && xDiff == 0 && y1 == 6) //moving 2 sqares
    {
        if (board[x1][y1 - 1] == 0 && board[x1][y1 - 2] == 0)
            return 1;
        else
            return 0;
    }

    if (abs(xDiff) == 1 && yDiff == 1) //taking a piece
    {
        if (board[x2][y2] > 0)
            return 1;
        else
            return 0;
    }

    if (yDiff == 1 && board[x1][y1 - 1] == 0)
        return 1;
    else
        return 0;

    if (abs(xDiff) > 0)
        return 0;
    if (yDiff > 1)
        return 0;
    if (yDiff < 0)
        return 0;

    return 1;
}

int knightCheck(int x1, int y1, int x2, int y2)
{
    //posible knight moves
    int X[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int Y[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    int xDiff = x1 - x2;
    int yDiff = y1 - y2;
    for (int i = 0; i < 8; i++)
        if (yDiff == Y[i] && xDiff == X[i])
            return 1;

    return 0;
}

int bishopCheck(int x1, int y1, int x2, int y2)
{
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;

    if (abs(xDiff) != abs(yDiff)) return 0;

    int xAux = xDiff / abs(xDiff);
    int yAux = yDiff / abs(yDiff);

    int j = y2;
    for (int i = x2+xAux; i != x1; i += xAux)
    {
        j += yAux;
        if (board[i][j] != 0) return 0;

    }

    return 1;
}

int rookCheck(int x1, int y1, int x2, int y2)
{
    return 1;
}

int queenCheck(int x1, int y1, int x2, int y2)
{
    return 1;
}

int kingCheck(int x1, int y1, int x2, int y2)
{
    return 1;
}