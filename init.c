#include "interface.h"

/*
0-15: pawns
16-19: knights
20-23: bishops
24-27: rooks
28-29: queens
30-31: kings
*/

/*adds all starting pieces to piece array. assumes enough memory is assigned already*/
int addAllPieces(Piece *allPieces)
{
    int index = 0;

    index = addPawns(allPieces, WHITE, index);
    index = addPawns(allPieces, BLACK, index);
    index = addTwoPieces(allPieces, index, 1, KNIGHT);
    index = addTwoPieces(allPieces, index, 6, KNIGHT);
    index = addTwoPieces(allPieces, index, 2, BISHOP);
    index = addTwoPieces(allPieces, index, 5, BISHOP);
    index = addTwoPieces(allPieces, index, 0, ROOK);
    index = addTwoPieces(allPieces, index, 7, ROOK);
    index = addTwoPieces(allPieces, index, 3, QUEEN);
    index = addTwoPieces(allPieces, index, 4, KING);

    return index;
}

/*adds pawns to the board for one colour*/
int addPawns(Piece *allPieces, int colour, int index)
{
    int end, i, row, column = 0;
    Piece newPawn;

    if (colour) { /*white*/
        row = 6;
    } else { /*black*/
        row = 1;
    }

    newPawn.colour = colour;
    newPawn.typeVal = PAWN;
    newPawn.pos.m = row;
    newPawn.mc = 0;

    end = index + 8;

    for (i = index; i < end; i++) {
        newPawn.pos.n = column;
        allPieces[i] = newPawn;
        column++;
    }

    return i;
}

/* adds a piece for both colours */
int addTwoPieces(Piece *allPieces, int index, int column, int typeVal)
{
    Piece newPiece;

    newPiece.pos.m = 7;
    newPiece.pos.n = column;
    newPiece.colour = WHITE;
    newPiece.mc = 0;
    newPiece.typeVal = typeVal;

    allPieces[index++] = newPiece;

    newPiece.pos.m = 0;
    newPiece.colour = BLACK;

    allPieces[index++] = newPiece;

    return index;
}
