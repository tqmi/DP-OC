#include <stdio.h>
#include "../logic/logic.h"

//for testing
int main()
{
    setStartingBoard();

    int auxBoard[8][8];
    getBoard(auxBoard);
    printBoard(auxBoard);
    
    int x1,x2,y1,y2;

    int x;
    do{
    printf("Pick piece: ");
    scanf("%d",&x1);
    scanf("%d",&y1);
    printf("Pick move: ");
    scanf("%d",&x2);
    scanf("%d",&y2);
    printf("\n\n\n");

    x = movePiece(x1, y1, x2, y2);

    getBoard(auxBoard);
    printBoard(auxBoard);
    if(x==0) printf("BAD MOVE\n");
    }while(1);

    return 0;
}