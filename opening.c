#include "interface.h"

int main(int argc, char **argv)
{
    Piece allPieces[LENGTH];
    Action computerMove;
    AB ab;
    int eval;
    int colour = WHITE;
    int start, end;



    ab.alpha = -2000000;
    ab.beta = 2000000;

    addAllPieces(allPieces);

    printBoard(allPieces);



    while(1) {
        movePrompt(allPieces);

        printBoard(allPieces);


        start = clock();
        computerMove = findMostEpicMove(ab,6,BLACK,allPieces);
        end = clock();
        executeMove(computerMove.piece, computerMove.move, allPieces);

        printf("\n");
        printf("%ds\n", (end - start) / CLOCKS_PER_SEC);
        printBoard(allPieces);

        colour = !colour;

    }

    return 0;
}
