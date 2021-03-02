
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>
#include "../libs/logic.h"
#include "UI.h"
#include "../libs/network.h"



void red()
{
    wprintf(L"\033[1;31m");
}

void reset()
{
    wprintf(L"\033[0m");
}

void init_UI()
{
     setlocale(LC_CTYPE, "");
}

void systemClear()
{
   
    system("clear");
}

void printMessage(char* message)
{
    systemClear();
    wprintf(L"%s\n", message);
}

int giveAnswer(char answer)
{
    if(toupper(answer) == 'N')
        return 0;
    if(toupper(answer) == 'Y')
        return 1;
    return -1;
}

wchar_t getPiecesSymbol(int piece)
{
   
    wchar_t chess_piece;

    switch (piece)
    {
    case WKING:
        chess_piece = WHITE_KING;
        
        break;
    case WQUEEN:
        chess_piece = WHITE_QUEEN;
        
        break;
    case WROOK:
        chess_piece = WHITE_ROOK;
       
        break;
    case WBISHOP:
        chess_piece = WHITE_BISHOP;
        
        break;
    case WKNIGHT:
        chess_piece = WHITE_KNITE;
        
        break;
    case WPAWN:
        chess_piece = WHITE_PAWN;
        
        break;
    case BKING:
        chess_piece = BLACK_KING;
        
        break;
    case BQUEEN:
        chess_piece = BLACK_QUEEN;
        
        break;
    case BROOK:
        chess_piece = BLACK_ROOK;
        
        break;
    case BBISHOP:
        chess_piece = BLACK_BISHOP;
        
        break;
    case BKNIGHT:
        chess_piece = BLACK_KNITE;
       
        break;
    case BPAWN:
        chess_piece = BLACK_PAWN;
        
        break;
    default:
        break;
    }
    return chess_piece;

}

void printBoard(int board[8][8])
{
    systemClear();
    int size = 8;
    int row = 8;
    char column = 'A';
    int row_board = 0;
    int column_board = 0;

    //Write the A B C.. series
   
    wprintf(L"          ");
    for (int i = 0; i < 8; i++)
    {
        red();
        wprintf(L"  %lc ", column);
        column += 1;
    }
    wprintf(L"\n");
    reset();
    // A B C columns end

    for (int i = 0; i < size; i++)
    {
        //Draw the top line of the table
        wprintf(L"          ");
        for (int j = 0; j < size; j++)
        {
            wprintf(L" ---");
        }
        wprintf(L"\n");
        //top line end

        //Draw the squares
        red();
        wprintf(L"       %ld  ", row);
        reset();
        for (int j = 0; j < size; j++)
        {
            if (board[row_board][column_board] == EMPTY)
            {
                wprintf(L"|   ");
            }
            else
            {
                wchar_t chess_piece = getPiecesSymbol(board[row_board][column_board]);
                wprintf(L"| ");
                wprintf(L"%lc ",chess_piece);
            
            }
            column_board += 1;
        }

        wprintf(L"|  ");
        red();
        wprintf(L"%ld\n", row);
        reset();
        row -= 1;
        row_board += 1;
        column_board = 0;

        //Squares end
    }

    //Draw the bottom line
    wprintf(L"          ");
    for (int j = 0; j < size; j++)
    {
        wprintf(L" ---");
    }
    wprintf(L"\n");
    //Bottom line end

    //Write the A B C.. series
    column = 'A';
    wprintf(L"          ");
    for (int i = 0; i < 8; i++)
    {
        red();
        wprintf(L"  %lc ", column);
        column += 1;
    }
    wprintf(L"\n");
    reset();
    // A B C columns end
    
}

int get_keyboard_input(char * buffer, int bufflen){
    int fd,rd;
    if((rd = read_data(buffer,bufflen,&fd)) > 0){
        if(fd == KEYBOARDIN){
            return rd;
        }else{
            // TODO!!
        }
	}
    return 0;
}


// int main()
// {

   
    
//     initUI();
//     int board[8][8] =
//         {

//             {-4, -2, -3, -5, -6, -3, -2, -4},
//             {-1, -1, -1, -1, -1, -1, -1, -1},
//             {0, 0, 0, 0, 0, 0, 0, 0},
//             {0, 0, 0, 0, 0, 0, 0, 0},
//             {0, 0, 0, 0, 0, 0, 0, 0},
//             {0, 0, 0, 0, 0, 0, 0, 0},
//             {1, 1, 1, 1, 1, 1, 1, 1},
//             {4, 2, 3, 5, 6, 3, 2, 4}};

//     printBoard(board);

//     int answer= giveAnswer('N');

//     wprintf(L"%ld\n",answer);


//     return 0;
    

    

    
// }
