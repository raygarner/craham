#include "interface.h"

int main(int argc, char **argv)
{
    Piece allPieces[LENGTH];
    Action computerMove;
    AB ab;
    int eval;
    int colour = WHITE;



    ab.alpha = -2000000;
    ab.beta = 2000000;

    addAllPieces(allPieces);

    printBoard(allPieces);



    while(1) {
/*         movePrompt(allPieces); */



/*         printBoard(allPieces); */



        computerMove = findMostEpicMove(ab,4,colour,allPieces);
        executeMove(computerMove.piece, computerMove.move, allPieces);
        printf("%d\n", computerMove.eval);

        printf("\n");
        printBoard(allPieces);
        printf("white: %d\tblack: %d\n", totalMaterial(WHITE, allPieces), totalMaterial(BLACK,allPieces));

        colour = !colour;

    }

    return 0;
}
