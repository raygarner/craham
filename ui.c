#include "interface.h"

int printBoard(Piece *allPieces)
{
    int m, n;
    Piece *piece;
    Tuple pos;

    printf("  ");
    for (m = 0; m < 8; m++) {
        printf("%d ", m);
    }
    printf("\n");

    for (m = 0; m < 8; m++) {
        pos.m = m;
        printf("%d ", m);
        for (n = 0; n < 8; n++) {
            pos.n = n;
            piece = findPiece(pos, allPieces);

            if (piece == NULL) {
                if (n % 2 == 0) {
                    if (m % 2 == 0)
                        printf("+");
                    else
                        printf("-");
                } else {
                    if (m % 2 == 0)
                        printf("-");
                    else
                        printf("+");
                }
            } else {
                printf("%c", getPieceIcon(*piece));
            }
            printf(" ");
        }
        printf("\n");
    }

    return 8 * m + n;
}

char getPieceIcon(Piece piece)
{
    int colour = piece.colour;
    int type = piece.typeVal;


    switch(type) {
    case KING :
        if (colour)
            return 'K';
        else
            return 'k';

    case QUEEN :
        if (colour)
            return 'Q';
        else
            return 'q';

    case ROOK :
        if (colour)
            return 'R';
        else
            return 'r';

    case BISHOP :
        if (colour)
            return 'B';
        else
            return 'b';

    case KNIGHT :
        if (colour)
            return 'N';
        else
            return 'n';

    case PAWN :
        if (colour)
            return 'P';
        else
            return 'p';

    default :
        return '?';
    }

    return '?';
}

Piece *movePrompt(Piece *allPieces)
{
    Tuple pos, move;
    Piece *toMove;

    printf("Enter the move you wish to make: ");
    scanf("%d %d %d %d", &pos.m, &pos.n, &move.m, &move.n);

    toMove = findPiece(pos, allPieces);

    if (toMove == NULL)
        return NULL;

    /*
    if (isValidMove(*toMove, move, allPieces) && \
    !willBeInCheck(*toMove, move, allPieces))
        executeMove(*toMove, move, allPieces);
    else
        printf("INVALID MOVE\n");
    */

    if (!isValidMove(*toMove, move, allPieces))
        printf("INVALID MOVE\n");
    else if (willBeInCheck(*toMove, move, allPieces))
        printf("YOU CANNOT MOVE INTO CHECK\n");
    else
        executeMove(*toMove, move, allPieces);

    return allPieces;
}
