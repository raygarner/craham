#include "interface.h"

/* returns the eval for a postion */
int totalVal(Piece *allPieces)
{
    int whiteEval, blackEval;

    whiteEval = totalMaterial(WHITE, allPieces) + evalAllPieces(WHITE, allPieces);
    blackEval = totalMaterial(BLACK, allPieces) + evalAllPieces(BLACK, allPieces);

    return whiteEval - blackEval;
}

/* returns the ammount of pieces of one type for a side */
int countPieceType(int colour, int typeVal, Piece *allPieces)
{
    int i, count = 0, max;

    /*
    switch(typeVal) {
    case PAWN :
        max = 8;
        break;
    case KNIGHT :
    case BISHOP :
    case ROOK :
        max = 4;
        break;

    case QUEEN :
        max = 3;
    case KING:
        max = 1;

    default :
        max = LENGTH;
    }
    */

    for (i = 0; i < LENGTH; i++) {
        if (allPieces[i].colour == colour && \
        allPieces[i].typeVal == typeVal && \
        isOnBoard(allPieces[i].pos)) {
            count++;

            /*
            if (count == max)
                return count;
            */
        }
    }

    return count;
}

/* returns the material evaluation for one side */
int totalMaterial(int colour, Piece *allPieces)
{
    static const int types[] = {100, 300, 350, 500, 900};
    int i, total = 0;

    /*
    for (i = 0; i < 5; i++) {
        total += types[i] * countPieceType(colour, types[i], allPieces);
    }
    */

    for (i = 0; i < LENGTH; i++) {
        if (allPieces[i].colour == colour && \
        isOnBoard(allPieces[i].pos)) {
            total += allPieces[i].typeVal;

        }

    }

    return total;
}

/* calculates the index of the pos for use with a bitboard */
int calcIndex(int colour, Tuple pos)
{
    int i = pos.m * 8 + pos.n;

    if (colour)
        return i;
    else
        return 63 - i;
}

/* evaluates all the piece on one side */
int evalAllPieces(int colour, Piece *allPieces)
{
    int sum = 0, i;

    for (i = 0; i < LENGTH; i++) {
        if (allPieces[i].colour == colour && \
        isOnBoard(allPieces[i].pos)) {
            sum += evalPiece(allPieces[i]);
        }
    }

    return sum;
}

/* returns the bitboard based eval for a piece */
int evalPiece(Piece piece)
{
    switch(piece.typeVal) {
    case KING :
        return evalKing(piece);

    case BISHOP :
        return evalBishop(piece);

    case KNIGHT :
        return evalKnight(piece);

    default :
        return 0;
    }

    return 0;
}

/* gets the value from the knight bitboard */
int evalKnight(Piece piece)
{

    /*
    static const int squares[] = {-50,-40,-30,-30,-30,-30,-40,-50,
                                -40,-20,  0,  0,  0,  0,-20,-40,
                                -30,  0, 10, 15, 15, 10,  0,-30,
                                -30,  5, 15, 10, 10, 15,  5,-30,
                                -30,  0, 15, 20, 20, 15,  0,-30,
                                -30,  5, 10, 15, 15, 10,  5,-30,
                                -40,-20,  0,  5,  5,  0,-20,-40,
                                -50,-40,-30,-30,-30,-30,-40,-50};
    int i = calcIndex(piece.colour, piece.pos);
    */

    return howCentral(piece.pos);
}

/* gets the corresponding value from the bishop bitboard */
int evalBishop(Piece piece)
{
    /*
    static const int squares[] = {-20,-10,-10,-10,-10,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5, 10, 10,  5,  0,-10,
                                -10,  5,  5, 10, 10,  5,  5,-10,
                                -10,  0, 10, 10, 10, 10,  0,-10,
                                -10, 10, 10, 10, 10, 10, 10,-10,
                                -10,  5,  0,  0,  0,  0,  5,-10,
                                -20,-10,-10,-10,-10,-10,-10,-20};
    int i = calcIndex(piece.colour, piece.pos);

    return squares[i];
    */
    return howCentral(piece.pos);
}

/* returns the corresponding value from the king bitboard */
int evalKing(Piece piece)
{
    /*
    static const int squares[] = {-30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -20,-30,-30,-40,-40,-30,-30,-20,
                                -10,-20,-20,-20,-20,-20,-20,-10,
                                 0, 0,  0,  -5,  -5,  -5, 0, 0,
                                 20, 30, 20,  0,  0, 0, 30, 20};
    int i = calcIndex(piece.colour, piece.pos);

    return squares[i];
    */

    if (piece.pos.n >= 6 || piece.pos.n <= 2)
        return 20;
    else
        return 0;
}

/* returns a value based on how central the piece is*/
int howCentral(Tuple pos)
{
    int m = lesser(pos.m, 7 - pos.m);
    int n = lesser(pos.n, 7 - pos.n);

    return 10 * (m + n);
}
