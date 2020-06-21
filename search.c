#include "interface.h"

/* returns the best move which can be made in a given position */
Action findMostEpicMove(AB ab, int depth, int colour, Piece *allPieces)
{
    Action action;
    Action legalMoves[MOVES];
    int index;

    if (depth == 0) {
        /*
        action.piece.typeVal = KING;
        action.piece.pos.m = 0;
        action.piece.pos.n = 0;
        action.piece.colour = colour;
        action.piece.mc = 0;
        action.move.m = 0;
        action.move.n = 0;
        */
        action.eval = totalVal(allPieces);

        return action;
    }

    index = addAllLegalMoves(colour,allPieces,legalMoves);
    addLastAction(legalMoves, index);
    addEvals(ab,depth,colour,allPieces,legalMoves);

    return strongestMoveFromList(colour,legalMoves);

}

/* adds true evals to a list of moves */
Action
*addEvals(AB ab, int depth, int colour, Piece *allPieces, Action *legalMoves)
{
    Piece newBoard[LENGTH];
    int eval, i = 0;
    Explore explore;
    copyBoard(allPieces, newBoard);

    while (!isLastAction(legalMoves[i])) {
        executeMove(legalMoves[i].piece, legalMoves[i].move, newBoard);
        eval = findMostEpicMove(ab, depth-1, !colour, newBoard).eval;
        explore = dontExplore(ab, colour, eval);


        if (explore.noex) {
            legalMoves[i].eval = explore.eval;
            addLastAction(legalMoves, i+1); /*mark this as end of list*/
            return legalMoves;
        } else {
            legalMoves[i].eval = eval;
            copyBoard(allPieces, newBoard);/*reset board*/
            ab = updateAB(ab,colour,eval);
            i++;
        }


        /*
        legalMoves[i].eval = eval;
        copyBoard(allPieces, newBoard);
        i++;
        */

    }

    return legalMoves;
}

/* returns whether a line should be explored and if not returns an eval*/
Explore dontExplore(AB ab, int colour, int eval)
{
    Explore explore;

/*     printf("%d %d\n", ab.alpha, ab.beta); */

    if (colour) {
        explore.eval = ab.beta;

        if (eval >= ab.beta)
            explore.noex = 1;
        else
            explore.noex = 0;
    } else {
        explore.eval = ab.alpha;

        if (eval <= ab.alpha)
            explore.noex = 1;
        else
            explore.noex = 0;
    }

    return explore;
}

/* updates the alpha and beta based on an eval */
AB updateAB(AB ab, int colour, int eval)
{

    if (colour) {
        if (eval > ab.alpha) {
            ab.alpha = eval;
        }
    } else {
        if (eval < ab.beta) {
            ab.beta = eval;
        }
    }

    return ab;
}

/* returns the action with the best eval */
Action strongestMoveFromList(int colour, Action *legalMoves)
{
    int i = 1;
    Action currentBest;

    currentBest = legalMoves[0];

    /* written this way to reduce colour case checks */
    /* even through its ugly*/
    if (colour) {
        while (!isLastAction(legalMoves[i])) {
            if (legalMoves[i].eval > currentBest.eval) {
                currentBest = legalMoves[i];
            }

            i++;
        }
    } else {
        while (!isLastAction(legalMoves[i])) {
            if (legalMoves[i].eval < currentBest.eval) {
                currentBest = legalMoves[i];
            }

            i++;
        }
    }

    return currentBest;
}

/* adds every legal move for one colour */
int addAllLegalMoves(int colour, Piece *allPieces, Action *legalMoves)
{
    int i, index = 0;

    for (i = 0; i < LENGTH; i++) {
        if (allPieces[i].colour == colour && isOnBoard(allPieces[i].pos)) {
            index = addLegalMoves(allPieces[i],allPieces,legalMoves,index);
        }
    }

    return index;
}

/* adds legal moves for one piece to legalMoves list */
int
addLegalMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{
    switch(piece.typeVal) {
    case KING :
        index = legalKingMoves(piece,allPieces,legalMoves,index);
        return index;

    case QUEEN :
        index = legalQueenMoves(piece,allPieces,legalMoves,index);
        return index;

    case ROOK :
        index = legalRookMoves(piece,allPieces,legalMoves,index);
        return index;

    case BISHOP :
        index = legalBishopMoves(piece,allPieces,legalMoves,index);
        return index;

    case KNIGHT :
        index = legalKnightMoves(piece,allPieces,legalMoves,index);
        return index;

    case PAWN :
        index = legalPawnMoves(piece,allPieces,legalMoves,index);
        return index;

    default :
        return index;
    }


    return index;
}


/* adds the legal moves that 1 pawn can make */
int
legalPawnMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{
    int i, m, n;
    int dir = pawnDirection(piece.colour);
    Tuple move;

    move.n = 0;

    move.m = dir * 2;

    if (isValidTarget(piece, move, allPieces) && \
    isPawnForwardTwo(piece, move, allPieces) && \
    !willBeInCheck(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
         legalMoves[index].eval = 0;
        index++;
    }

    move.m = dir;

    if (isValidTarget(piece, move, allPieces) && \
    isBasicPawnMove(piece.colour, move) && \
    !willBeInCheck(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
         legalMoves[index].eval = 0;
        index++;
    }

    for (i = -1; i <= 1; i += 2) {
        move.n = i;

        if (isPawnValidMove(piece, move, allPieces) && \
        !willBeInCheck(piece, move, allPieces)) {
            legalMoves[index].piece = piece;
            legalMoves[index].move = move;
            legalMoves[index].eval = 0;
            index++;
        }

        if (isValidEnPassant(piece, move, allPieces) && \
        !willBeInCheck(piece, move, allPieces)) {
            legalMoves[index].piece = piece;
            legalMoves[index].move = move;
            legalMoves[index].eval = 0;
            index++;
        }

    }

    return index;
}

/* adds teh legal moves that 1 knight can make */
int
legalKnightMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{

    index = knightMoveLegality(piece,1,2,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,-1,2,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,1,-2,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,-1,-2,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,2,1,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,-2,1,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,2,-1,allPieces,legalMoves,index);
    index = knightMoveLegality(piece,-2,-1,allPieces,legalMoves,index);

    return index;
}

/* if a knight move is legal then its added to the list of legal moves */
int
knightMoveLegality(Piece piece, int m, int n, Piece *allPieces, \
Action *legalMoves, int index)
{
    Tuple move;

    move.m = m;
    move.n = n;

    if (isKnightValidMove(piece, move, allPieces) && \
    !willBeInCheck(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
    }

    return index;
}

/* adds the legal moves that 1 bishop can make */
int
legalBishopMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{

    index = bishopLineLegality(piece,-1,-1,allPieces,legalMoves,index);
    index = bishopLineLegality(piece,1,-1,allPieces,legalMoves,index);
    index = bishopLineLegality(piece,-1,1,allPieces,legalMoves,index);
    index = bishopLineLegality(piece,1,1,allPieces,legalMoves,index);

    return index;
}

/* adds the legal moves from one bishop direction */
int
bishopLineLegality(Piece piece, int m, int n, Piece *allPieces, \
Action *legalMoves, int index)
{
    Tuple move;

    move.m = m;
    move.n = n;

    if (willBeInCheck(piece, move, allPieces))
        return index;

    while (isBishopValidMove(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
        move.m = furtherFromZero(move.m);
        move.n = furtherFromZero(move.n);
    }

    return index;
}

/* adds the legal moves than 1 rook can make */
int
legalRookMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{

    index = rookLineLegality(piece,1,0,allPieces,legalMoves,index);
    index = rookLineLegality(piece,-1,0,allPieces,legalMoves,index);
    index = rookLineLegality(piece,0,1,allPieces,legalMoves,index);
    index = rookLineLegality(piece,0,-1,allPieces,legalMoves,index);

    return index;
}

/* adds teh legal moves from one rook direction */
int
rookLineLegality(Piece piece, int m, int n, Piece *allPieces, \
Action *legalMoves, int index)
{
    Tuple move;

    move.m = m;
    move.n = n;

    if (willBeInCheck(piece, move, allPieces))
        return index;

    while (isRookValidMove(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
        move = increaseStraightMove(move);
    }

    return index;
}

/* adds the legal moves that one queen can make */
int
legalQueenMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{
    index = legalRookMoves(piece, allPieces, legalMoves, index);
    index = legalBishopMoves(piece, allPieces, legalMoves, index);

    return index;
}

/* adds the legal moves that one king can make */
int
legalKingMoves(Piece piece, Piece *allPieces, Action *legalMoves, int index)
{
    Tuple move;

    index = kingMoveLegality(piece,1,1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,0,1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,1,0,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,-1,-1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,1,-1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,-1,1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,0,-1,allPieces,legalMoves,index);
    index = kingMoveLegality(piece,-1,0,allPieces,legalMoves,index);

    move.m = 0;
    move.n = 2;

    if (validCastle(piece,move,allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
    }

    move.n = -2;

    if (validCastle(piece,move,allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
    }

    return index;
}

/* adds a move if it is a legal king move */
int
kingMoveLegality(Piece piece, int m, int n, Piece *allPieces, \
Action *legalMoves, int index)
{
    Tuple move;

    move.m = m;
    move.n = n;

    if (validKingMove(piece, move, allPieces) && \
    !willBeInCheck(piece, move, allPieces)) {
        legalMoves[index].piece = piece;
        legalMoves[index].move = move;
        legalMoves[index].eval = 0;
        index++;
    }

    return index;
}

/* adds the marked to say there are no more legal moves*/
Action *addLastAction(Action *legalMoves, int index)
{
    legalMoves[index] = LASTACTION;

    return legalMoves;
}


