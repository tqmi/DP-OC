
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logic.h"

void resizeCoord(int* x1, int* y1, int* x2, int* y2);

void movePieceDirectly(int x1, int y1, int x2, int y2);

int promotePawn(int newPiece);

int fullCheck(int x1, int y1, int x2, int y2);
int basicCheck(int x1, int y1, int x2, int y2);
int pawnPromotionCheck(int x1, int y1, int x2, int y2);

int pawnCheck(int x1, int y1, int x2, int y2);
int blackPawnCheck(int x1, int y1, int x2, int y2);
int whitePawnCheck(int x1, int y1, int x2, int y2);
int knightCheck(int x1, int y1, int x2, int y2);
int bishopCheck(int x1, int y1, int x2, int y2);
int rookCheck(int x1, int y1, int x2, int y2);
int queenCheck(int x1, int y1, int x2, int y2);
int kingCheck(int x1, int y1, int x2, int y2);
int kingCastleCheck(int x1, int y1, int x2, int y2);

int isBlackKingChecked();
int isWhiteKingChecked();
int isKingChecked();
int canPieceStopCheck(int x1, int y1);
int isMate();

int board[8][8];
int playerTurn;
int whiteKingMoved; //used for castling
int blackKingMoved;

void initLogic()
{
    setStartingBoard();
}

void setStartingBoard()
{
    //Pawns
    for (int i = 0; i < 8; i++)
    {
        board[i][1] = WPAWN;
        board[i][6] = BPAWN;
    }

    //Knights
    board[1][0] = board[6][0] = WKNIGHT;
    board[1][7] = board[6][7] = BKNIGHT;

    //Bishops
    board[2][0] = board[5][0] = WBISHOP;
    board[2][7] = board[5][7] = BBISHOP;

    //Rooks
    board[0][0] = board[7][0] = WROOK;
    board[0][7] = board[7][7] = BROOK;

    //Queens
    board[3][0] = WQUEEN;
    board[3][7] = BQUEEN;

    //Kings
    board[4][0] = WKING;
    board[4][7] = BKING;

    playerTurn = 1;
    whiteKingMoved = 0;
    blackKingMoved = 0;
}

//copies the board into the board given as a parameter
void getBoard(int auxBoard[8][8])
{
    memcpy(auxBoard, board, 8 * 8 * sizeof(int));
}


//returns 0 on invalid move
//returns 1 for valid move
//returns 2 for valid move with check
//returns 3 for valid move with mate
//returns 4 for valid move with pawn promotions
// x1,y1 - initial position, can have values between [1,8]
// x2,y2 - new position
int movePiece(int x1, int y1, int x2, int y2)
{
    resizeCoord(&x1, &y1, &x2, &y2);

    if (!fullCheck(x1, y1, x2, y2))
        return 0;

    //in case of pawn promotion / castling
    if (pawnPromotionCheck(x1, y1, x2, y2))
    {
            movePieceDirectly(x1, y1, x2, y2);
            return 4; //pawn promotion
        
    }

    movePieceDirectly(x1, y1, x2, y2);
    playerTurn *= -1; //next player turn

    if (isKingChecked())
    {
        if (isMate())
            return 3;
        else
            return 2;
    }
    else
        return 1;
}

//moves piece with no regards to rules
void movePieceDirectly(int x1, int y1, int x2, int y2)
{
    //moving piece
    int pieceType = board[x1][y1];
    board[x2][y2] = pieceType;
    board[x1][y1] = EMPTY;

    if (pieceType == 6)
    {
        whiteKingMoved = WKING;
    }
    else if (pieceType == BKING)
    {
        blackKingMoved = 1;
    }

   
}

//return 1 if its WHITE's turn and -1 for BLACK
int getPlayerTurn()
{
    return playerTurn;
}

//makes the coord from user frendly to array frendly
void resizeCoord(int* x1, int* y1, int* x2, int* y2)
{
    *x1 -= 'a';
    *y1 -= 1;
    *x2 -= 'a';
    *y2 -= 1;
}

//combines basic check and king check rules into one
int fullCheck(int x1, int y1, int x2, int y2)
{
    if (!basicCheck(x1, y1, x2, y2))
        return 0;

    //checks if the king will still be checked after making the move
    int pieceStart = board[x1][y1];
    int pieceEnd = board[x2][y2];
    board[x2][y2] = pieceStart;
    board[x1][y1] = EMPTY;
    if (isKingChecked())
    {
        board[x2][y2] = pieceEnd;
        board[x1][y1] = pieceStart;
        return 0;
    }
    board[x2][y2] = pieceEnd;
    board[x1][y1] = pieceStart;

    return 1;
}

//checks general things that are valid for all pieces(not EMPTY piece, boundaries)
//and more advanced things(piece movement, if path of piece is not blocked)
// return 0 on incorrect imput, 1 otherwise
int basicCheck(int x1, int y1, int x2, int y2)
{
    //not outside of board check
    if (x1 > 7 || y1 > 7 || x2 > 7 || y2 > 7)
        return 0;
    if (x1 < 0 || x1 < 0 || x1 < 0 || x1 < 0)
        return 0;

    //non EMPTY start check
    if (board[x1][y1] == 0)
        return 0;

    //not ending on same color piece check
    if (board[x2][y2] != 0)
    {
        int pieceColorStart = (board[x1][y1] > 0) ? WHITE : BLACK; 
        int pieceColorEnd = (board[x2][y2] > 0) ? WHITE : BLACK;

        if (pieceColorStart == pieceColorEnd)
            return 0;
    }

    int pieceType = board[x1][y1];

    //in case a player tries to move the enemy pieces
    if (pieceType * playerTurn < 0)
        return 0;

    switch (pieceType)
    {
    case BPAWN:
    case WPAWN:
        if (!pawnCheck(x1, y1, x2, y2))
            return 0;
        break;

    case BKNIGHT:
    case WKNIGHT:
        if (!knightCheck(x1, y1, x2, y2))
            return 0;
        break;

    case BBISHOP:
    case WBISHOP:
        if (!bishopCheck(x1, y1, x2, y2))
            return 0;
        break;

    case BROOK:
    case WROOK:
        if (!rookCheck(x1, y1, x2, y2))
            return 0;
        break;

    case BQUEEN:
    case WQUEEN:
        if (!queenCheck(x1, y1, x2, y2))
            return 0;
        break;

    case BKING:
    case WKING:
        if (!kingCheck(x1, y1, x2, y2))
            return 0;
        break;

    default:
        return 0;
    }

    return 1;
}

//pawnPromotion/En passant/castling
int pawnPromotionCheck(int x1, int y1, int x2, int y2)
{
    int pieceType = board[x1][y1];

    if (abs(pieceType) == WPAWN) //BLACK or WHITE, dosent matter
    {
        //pawn promotion
        if (playerTurn == WHITE && y2 == 7)
            return 1;
        else if (playerTurn == BLACK && y2 == 0)
            return 1;
        else
            return 0;
    }

    else return 0;
}

//promotes pawn to new piece
//use values from [2,5], will be adjusted automaticaly depending on player turn
//return 1 for valid piece value, 0 otherwise
int promotePawn(int newPiece)
{
    if (newPiece > 5 || newPiece < 2)
        return 0;

    // the player turn changes before promotion takes place
    int playerTurnAux = playerTurn * (-1);
    if (playerTurnAux == WHITE)
    {
        for (int i = 0; i < 8; i++)
        {
            //find pawn on row
            if (board[i][7] == WPAWN)
            {
                board[i][7] = newPiece;
                return 1;
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            //find pawn on row
            if (board[i][0] == BPAWN)
            {
                board[i][0] = newPiece;
                return 1;
            }
        }
    }
    return 0;
}

int pawnCheck(int x1, int y1, int x2, int y2)
{
    int pawnColor = board[x1][y1];

    if (pawnColor == WHITE)
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
        if (board[x1][y1 + 1] == EMPTY && board[x1][y1 + 2] == EMPTY)
            return 1;
        else
            return 0;
    }

    if (abs(xDiff) == 1 && yDiff == -1) //taking a piece
    {
        if (board[x2][y2] == BLACK)
            return 1;
        else
            return 0;
    }

    if (yDiff == -1 && board[x1][y1 + 1] == EMPTY)
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
        if (board[x1][y1 - 1] == EMPTY && board[x1][y1 - 2] == EMPTY)
            return 1;
        else
            return 0;
    }

    if (abs(xDiff) == 1 && yDiff == 1) //taking a piece
    {
        if (board[x2][y2] == WHITE)
            return 1;
        else
            return 0;
    }

    if (yDiff == 1 && board[x1][y1 - 1] == EMPTY)
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
    int X[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int Y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

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

    if (abs(xDiff) != abs(yDiff))
        return 0;

    int xAux = xDiff / abs(xDiff);
    int yAux = yDiff / abs(yDiff);

    //checking for pieces blocking the way
    int j = y2;
    for (int i = x2 + xAux; i != x1; i += xAux)
    {
        j += yAux;
        if (board[i][j] != EMPTY)
            return 0;
    }

    return 1;
}

int rookCheck(int x1, int y1, int x2, int y2)
{
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;

    if (xDiff == 0)
    {
        int yAux = yDiff / abs(yDiff);
        for (int i = y2 + yAux; i != y1; i += yAux)
            if (board[x1][i] != EMPTY)
                return 0;
    }
    else if (yDiff == 0)
    {
        int xAux = xDiff / abs(xDiff);
        for (int i = x2 + xAux; i != x1; i += xAux)
            if (board[i][y1] != EMPTY)
                return 0;
    }
    else
        return 0;

    return 1;
}

int queenCheck(int x1, int y1, int x2, int y2)
{
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;

    if (xDiff == 0 || yDiff == 0)
        return rookCheck(x1, y1, x2, y2);
    else
        return bishopCheck(x1, y1, x2, y2);
}

int kingCheck(int x1, int y1, int x2, int y2)
{
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;


    if (kingCastleCheck(x1, y1, x2, y2))//special case - castle
    {

        if (x1 < x2) //king side castle
            movePieceDirectly(x1 + 3, y1, x1 + 1, y1);//move rook
        else //queen side castle
            movePieceDirectly(x1 - 4, y1, x1-1, y2);

        return 1;
    }
    else if (abs(xDiff) > 1 || abs(yDiff) > 1)
        return 0;
    else
        return queenCheck(x1, y1, x2, y2);
}

//can king castle?
int kingCastleCheck(int x1, int y1, int x2, int y2)
{
    if (playerTurn == WHITE && whiteKingMoved)
        return 0;
    else if (playerTurn == BLACK && blackKingMoved)
        return 0;


    int xDiff = x1 - x2;

    if (xDiff == -2) //king side castle
    {
        //WHITE
        if (playerTurn == WHITE && board[5][0] == EMPTY && board[6][0] == EMPTY && board[7][0] == WROOK)
            return 1;

        //BLACK
        else if (playerTurn == BLACK && board[5][7] == EMPTY && board[6][7] == EMPTY && board[7][7] == BROOK)
            return 1;

        else
            return 0;
    }
    else if (xDiff == 2) //queen side castle
    {
        //WHITE
        if (playerTurn == WHITE && board[0][0] == WROOK && board[1][0] == EMPTY && board[2][0] == EMPTY && board[3][0] == EMPTY)
            return 1;

        //BLACK
        else if (playerTurn == BLACK && board[0][7] == BROOK && board[1][7] == EMPTY && board[2][7] == EMPTY && board[3][7] == EMPTY)
            return 1;

        else
            return 0;
    }
    else
        return 0;
}

//-1 color for BLACK, 1 for WHITE
void findKing(int color, int* kX, int* kY)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[j][i] == 6 * color)
            {
                *kX = j;
                *kY = i;
                return;
            }
}

int isWhiteKingChecked()
{
    int kX, kY; //king location
    findKing(WHITE, &kX, &kY);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (basicCheck(j, i, kX, kY))
                return 1;

    return 0;
}

int isBlackKingChecked()
{
    int kX, kY; //king location
    findKing(BLACK, &kX, &kY);

    for (int i = 7; i >= 0; i--)
        for (int j = 0; j < 8; j++)
            if (basicCheck(j, i, kX, kY))
                return 1;

    return 0;
}

//returns 1 if king is checked, 0 otherwise
int isKingChecked()
{
    if (getPlayerTurn() > 0)
        return isWhiteKingChecked();
    else
        return isBlackKingChecked();
}

int canPieceStopCheck(int x1, int y1)
{

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (fullCheck(x1, y1, i, j))
                return 1;
        }

    return 0;
}

int isMate()
{
    int color = playerTurn;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {

            if (board[i][j] * color > 0)
            {
                if (canPieceStopCheck(i, j))
                    return 0;
            }
        }

    return 1;
}

void setTestBoard()
{
    //Pawns
    for (int i = 0; i < 8; i++)
    {
        board[i][1] = WPAWN;
        board[i][6] = BPAWN;
    }

    /*//Knights
    board[1][0] = board[6][0] = WKNIGHT;
    board[1][7] = board[6][7] = BKNIGHT;*/

    /*//Bishops
    board[2][0] = board[5][0] = WBISHOP;
    board[2][7] = board[5][7] = BBISHOP;*/

    //Rooks
    board[0][0] = board[7][0] = WROOK;
    board[0][7] = board[7][7] = BROOK;

    /*//Queens
    board[3][0] = WQUEEN;
    board[3][7] = BQUEEN;*/

    //Kings
    board[4][0] = WKING;
    board[4][7] = BKING;

    playerTurn = 1;
    whiteKingMoved = 0;
    blackKingMoved = 0;
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

//for testing
/*int main()
{
    setTestBoard();

    int auxBoard[8][8];
    getBoard(auxBoard);
    printBoard(auxBoard);

    int x1, x2;
    int y1, y2;

    int x;
    do
    {
        if (getPlayerTurn() > 0)
            printf("WHITE TURN\n");
        else
            printf("BLACK TURN\n");
        printf("Pick piece: ");
        scanf("%d", &x1);
        scanf("%d", &y1);
        printf("Pick move: ");
        scanf("%d", &x2);
        scanf("%d", &y2);
        printf("\n\n\n");

        x = movePiece(x1, y1, x2, y2);

        if (x == 4)
        {
            if (promotePawn(5))
                printf("GOOD Promote\n");
            else
                printf("BAD Promote\n");
        }

        getBoard(auxBoard);
        printBoard(auxBoard);

        if (x == 0)
            printf("BAD MOVE\n");
        else if (x == 2)
            printf("CHEKED\n");
        else if (x == 3)
            printf("MATE\n");

    } while (x != 3);

    return 0;
}*/