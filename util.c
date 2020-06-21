#include "interface.h"

/* used to mark the end of a valid moves list */
const Action LASTACTION = NULL_ACTION;

/*calculates the difference between two positions*/
Tuple getTarget(Tuple pos, Tuple move)
{
    Tuple newpos;

    newpos.m = pos.m + move.m;
    newpos.n = pos.n + move.n;

    return newpos;
}

/*returns the move required to go from pos start to pos end*/
Tuple moveMade(Tuple start, Tuple end)
{
    Tuple move;

    move.m = end.m - start.m;
    move.n = end.n - start.n;

    return move;
}

/*returns true if two poss are equal*/
int compareTuple(Tuple a, Tuple b)
{
    return a.m == b.m && a.n == b.n;
}

/*returns an int one further from zero than the int passed*/
int furtherFromZero(int num)
{
    int further;

    if (num < 0) {
        further = num - 1;
    } else if (num > 0) {
        further = num + 1;
    } else {
        further = num;
    }

    return further;
}

/*returns an int one closer to zero than the int passed*/
int closerToZero(int num)
{

    if (num < 0) {
        return num + 1;
    } else if (num > 0) {
        return num - 1;
    } else {
        return num;
    }

}

/*returns a pointer to a piece on a specified position on the board*/
Piece *findPiece(Tuple pos, Piece *allPieces)
{
    int i;
    Piece currentPiece;

    for (i = 0; i < LENGTH; i++) {
        if (compareTuple(pos, allPieces[i].pos)) {
            return &allPieces[i];
        }
    }

    return NULL;
}

/*returns whether a square is empty or not*/
int isEmpty(Tuple pos, Piece *allPieces)
{
    Piece *contents = findPiece(pos, allPieces);

    return contents == NULL;
}

/*returns whether an integer is whithin the bounds of a chess board*/
int isInBounds(int x)
{
    return x >= 0 && x <= 7;
}

/*returns whether a position is on a chess board*/
int isOnBoard(Tuple pos)
{
    return (isInBounds(pos.m) && isInBounds(pos.n));
}

/*return wether universal chess rules are followed by a move*/
int isValidTarget(Piece piece, Tuple move, Piece *allPieces)
{
    Tuple targetPos = getTarget(piece.pos, move);
    Piece *targetPiece = findPiece(targetPos, allPieces);

    /*on board and target is not a friendly*/
    return isOnBoard(piece.pos) && isOnBoard(targetPos) && \
        (targetPiece == NULL || targetPiece->colour != piece.colour);
}

/*returns whether a diagonal move path is clear
(assumes move m and n are equal)*/
int isDiagonalMovePathEmpty(Tuple pos, Tuple move, Piece *allPieces)
{
    Tuple target;

    while (move.m != 0) {
        target = getTarget(pos, move);

        if(!isEmpty(target, allPieces))
            return 0;

        move = decreaseDiagonalMove(move);
    }

    return 1;
}

/*closerToZeros both elements of a tuple*/
Tuple decreaseDiagonalMove(Tuple move)
{
    move.m = closerToZero(move.m);
    move.n = closerToZero(move.n);

    return move;
}

/*closerToZeros the non-zero element of a move*/
Tuple decreaseStraightMove(Tuple move)
{
    if (move.m == 0) {
        move.n = closerToZero(move.n);
    } else {
        move.m = closerToZero(move.m);
    }

    return move;
}

/* furhterFromZeros the non-zero element of a move */
Tuple increaseStraightMove(Tuple move)
{
    if (move.m == 0) {
        move.n = furtherFromZero(move.n);
    } else {
        move.m = furtherFromZero(move.m);
    }

    return move;
}

/*returns whether a straight move path is clear*/
/*assumes path is actually straight*/
int isStraightMovePathEmpty(Tuple pos, Tuple move, Piece *allPieces)
{
    Tuple target;

    while (move.m != move.n) {
        target = getTarget(pos, move);

        if (!isEmpty(target, allPieces))
            return 0;

        move = decreaseStraightMove(move);
    }

    return 1;
}

/*returns whether a move is straight*/
int isStraightMove(Tuple move)
{
    return move.m == 0 || move.n == 0;
}

/*returns whether a move is diagonal*/
int isDiagonalMove(Tuple move)
{
    return abs(move.m) == abs(move.n);
}

/*returns whether a move is LShaped*/
int isLShaped(Tuple move)
{
    int m = abs(move.m);
    int n = abs(move.n);

    return (m == 2 && n == 1) || (m == 1 && n == 2);
}

/*returns whether a move is valid for a knight*/
int isKnightValidMove(Piece piece, Tuple move, Piece *allPieces)
{
   return isValidTarget(piece, move, allPieces) && isLShaped(move);
}

/*returns whether a move is valid for a bishop*/
int isBishopValidMove(Piece piece, Tuple move, Piece *allPieces)
{
    return isDiagonalMove(move) && isValidTarget(piece, move, allPieces) && \
    isDiagonalMovePathEmpty(piece.pos, decreaseDiagonalMove(move), allPieces);
}

/*returns whether a move is valid for a rook*/
int isRookValidMove(Piece piece, Tuple move, Piece *allPieces)
{
    return isStraightMove(move) && isValidTarget(piece, move, allPieces) && \
    isStraightMovePathEmpty(piece.pos, decreaseStraightMove(move), allPieces);
}

/*returns whether a move is valid for a queen*/
int isQueenValidMove(Piece piece, Tuple move, Piece *allPieces)
{
    return isRookValidMove(piece, move, allPieces) || \
        isBishopValidMove(piece, move, allPieces);
}

/*returns the direction that pawns of colour are heading*/
int pawnDirection(int colour)
{
    if (colour)
        return -1;
    else
        return 1;
}

/*returns whether a move is a pawn capture for a pawn of colour colour*/
int isPawnCapture(int colour, Tuple move)
{
    return move.m == pawnDirection(colour) && abs(move.n) == 1;
}

/*returns whether a move is a standard move for a pawn*/
int isBasicPawnMove(int colour, Tuple move)
{
    return move.m == pawnDirection(colour) && move.n == 0;
}

/*returns the home row for pawns of colour 'colour'*/
int pawnHomeRow(int colour)
{
    if (colour)
        return 6;
    else
        return 1;
}
/*returns whether a pawn is validly moving forward two*/
int isPawnForwardTwo(Piece pawn, Tuple move, Piece *allPieces)
{
    /*note move doesnt need to be decreased here like when being checked if a
    rook move is clear since the target square must be empty anyway*/
    return move.m == 2 * pawnDirection(pawn.colour) && move.n == 0 && \
        pawn.pos.m == pawnHomeRow(pawn.colour) && \
        isStraightMovePathEmpty(pawn.pos, move, allPieces);
}

/*returns whether a move is a valid en passant capture
(seperate from validpawnmove)*/
int isValidEnPassant(Piece pawn, Tuple move, Piece *allPieces)
{
    Piece *victim;
    Tuple victimPos;

    if (!isValidTarget(pawn, move, allPieces))
        return 0;

    if (!isPawnCapture || pawn.typeVal != PAWN)
        return 0;

    victimPos.m = pawn.pos.m;
    victimPos.n = pawn.pos.n + move.n;
    victim = findPiece(victimPos, allPieces);

    if (victim == NULL)
        return 0;

    return victim->mc == 2 && victim->colour != pawn.colour;
}

/*returns whether a pawn move is valid (doesnt include en passant)*/
int isPawnValidMove(Piece piece, Tuple move, Piece *allPieces)
{
    Piece *targetPiece;
    Tuple targetPos;

    if (!isValidTarget(piece, move, allPieces))
        return 0;

    targetPos = getTarget(piece.pos, move);
    targetPiece = findPiece(targetPos, allPieces);

    if (targetPiece == NULL) {
        if (isBasicPawnMove(piece.colour, move)) {
            return 1;
        } else if (isPawnForwardTwo(piece, move, allPieces)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (targetPiece->colour != piece.colour && \
        isPawnCapture(piece.colour, move)) {
            return 1;
        } else {
            return 0;
        }
    }

}

/*returns whether a move with validly mean a the pawn is promoted*/
int isValidPromotion(Piece piece, Tuple move, Piece *allPieces)
{
    return piece.typeVal == PAWN && isPawnValidMove(piece, move, allPieces) \
        && piece.pos.m == pawnHomeRow(!piece.colour);
}

/*returns whether the rook on a particular side has moved yet*/
int possibleToCastle(int colour, int side, Piece *allPieces)
{
    Piece *rook;
    Tuple rookPos;

    if (side) { /* if kingside castle*/
        rookPos.n = 7;
    } else {
        rookPos.n = 0;
    }

    if (colour) { /* if white castle */
        rookPos.m = 7;
    } else {
        rookPos.n = 0;
    }

    rook = findPiece(rookPos, allPieces);

    if (rook == NULL || rook->typeVal != ROOK || rook->mc != 0) {
        return 0;
    } else {
        return 1;
    }
}

/* returns whether a castling move is legal */
int validCastle(Piece piece, Tuple move, Piece *allPieces)
{
    int side;
    int start, end;
    int i;
    Tuple pos;

    if (move.m != 0 || abs(move.n) != 2 || piece.typeVal != KING)
        return 0;


    if (move.n == 2) { /* kingside castle*/
        side = 1;
    } else {
        side = 0;
    }

    if(!isStraightMovePathEmpty(piece.pos, move, allPieces))
        return 0;


    if (!possibleToCastle(piece.colour, side, allPieces))
        return 0;

    /*if safe castle path*/
    if (side) {
        start = 4;
        end = 6;
    } else {
        start = 2;
        end = 4;
    }

    if (piece.colour) { /*if white*/
        pos.m = 7;
    } else {
        pos.n = 0;
    }

    for (i = start; i <= end; i++) {
        pos.n = i;

        if (isThreatened(pos, piece.colour, allPieces)) {
            return 0;
        }
    }

    return 1; /*valid castle*/
}

/* returns whether a position is a valid move for any opposing piece
(other than king)*/
int isThreatened(Tuple pos, int colour, Piece *allPieces)
{
    int i;
    Piece currentPiece;
    Tuple move;

    for (i = 0; i < LENGTH; i++) {
        currentPiece = allPieces[i];
        move = moveMade(currentPiece.pos, pos);

        if (currentPiece.typeVal != KING && \
        isValidMove(currentPiece, move, allPieces) && \
        currentPiece.colour != colour) {
            return 1; /*pos is threatened*/
        }

    }

    return 0; /*no enemies threaten pos*/
}

/*returns whether a move is valid or not*/
int isValidMove(Piece piece, Tuple move, Piece *allPieces)
{
    int typeVal = piece.typeVal;

    switch(typeVal) {
    case PAWN :
        return isPawnValidMove(piece, move, allPieces) || \
            isValidEnPassant(piece, move, allPieces);

    case KNIGHT :
        return isKnightValidMove(piece, move, allPieces);

    case BISHOP :
        return isBishopValidMove(piece, move, allPieces);

    case ROOK :
        return isRookValidMove(piece, move, allPieces);

    case QUEEN :
        return isQueenValidMove(piece, move, allPieces);

    case KING :
        return validKingMove(piece, move, allPieces) || \
            validCastle(piece, move, allPieces);

    default :
        return 0;
    }

    return 0;
}

/*returns a pointer to the king of the specified colour*/
Piece *findKing(int colour, Piece *allPieces)
{
    Piece currentPiece;
    int i;

    for (i = 0; i < LENGTH; i++) {
        currentPiece = allPieces[i];

        if (currentPiece.typeVal == KING && currentPiece.colour == colour) {
            return &allPieces[i];
        }

    }

    return NULL;
}

/*returns whether a colour is in check*/
int inCheck(int colour, Piece *allPieces)
{
    Piece *king = findKing(colour, allPieces);

    if (king == NULL)
        return -1;

    return isThreatened(king->pos, colour, allPieces);
}

/*returns whether a move is valid for a king
(not including castling*/
int validKingMove(Piece king, Tuple move, Piece *allPieces)
{
    Piece *enemyKing;
    Tuple targetPos, diff;

    if (abs(move.m) > 1 || abs(move.n) > 1)
        return 0;

    if (!isValidTarget(king, move, allPieces))
        return 0;

    enemyKing = findKing(!king.colour, allPieces);

    if (enemyKing == NULL)
        return -1;

    targetPos = getTarget(king.pos, move);

    diff.m = abs(targetPos.m - enemyKing->pos.m);
    diff.n = abs(targetPos.n - enemyKing->pos.n);

    return diff.m > 1 || diff.n > 1;
}

/* returns a piece with updated position and mc based on a move */
Piece updatePosAndMc(Piece piece, Tuple move)
{
    int inc;

    piece.pos.m += move.m;
    piece.pos.n += move.n;

    if (piece.typeVal == PAWN && abs(move.m) == 2) {
        inc = 2;
    } else {
        inc = 1;
    }

    piece.mc += inc;

    return piece;
}

/* updates allPieces based on a move being made
(assumes move is valid)*/
Piece *executeMove(Piece piece, Tuple move, Piece *allPieces)
{
    Piece *toMove;
    Piece *targetPiece;
    Tuple targetPos;

    if (isValidPromotion(piece, move, allPieces)) {
        promotePawn(piece, move, allPieces);
        resetEnemyPawns(piece.colour, allPieces);
        return allPieces;
    }

    if (validCastle(piece, move, allPieces)) {
        executeCastle(piece, move, allPieces);
        resetEnemyPawns(piece.colour, allPieces);
        return allPieces;
    }


    if (isValidEnPassant(piece, move, allPieces)) {
        captureEnPassant(piece, move, allPieces);
        resetEnemyPawns(piece.colour, allPieces);
        return allPieces;
    }

    targetPos = getTarget(piece.pos, move);
    targetPiece = findPiece(targetPos, allPieces);

    if (targetPiece != NULL) {
        targetPiece->pos.m = CAPTURED;
        targetPiece->pos.n = CAPTURED;
    }

    toMove = findPiece(piece.pos, allPieces);

    if (toMove == NULL)
        return NULL;

    *toMove = updatePosAndMc(piece, move);

    resetEnemyPawns(piece.colour, allPieces);

    return allPieces;
}

/* resets the movecount for enemy pawns */
Piece *resetEnemyPawns(int colour, Piece *allPieces)
{
    int i;
    int count = 0;

    for (i = 0; i < LENGTH; i++) {
        if (allPieces[i].colour != colour && allPieces[i].typeVal == PAWN) {
            allPieces[i].mc = 0;
            count++;
        }

        if (count >= 8)
            return allPieces;
    }

    return allPieces;
}

/* updates allPieces with a castle move */
Piece *executeCastle(Piece piece, Tuple move, Piece *allPieces)
{
    Piece *toMove;
    Tuple rookPos;
    Tuple rookMove;

    toMove = findPiece(piece.pos, allPieces);

    if (toMove == NULL)
        return NULL;

    *toMove = updatePosAndMc(piece, move);

    if (move.n == 2) { /*kingside castle*/
        rookPos.n = 7;
        rookMove.n = -2;
    } else {
        rookPos.n = 0;
        rookMove.n = 3;
    }

    if (piece.colour) {
        rookPos.m = 7;
    } else {
        rookPos.m = 0;
    }

    toMove = findPiece(rookPos, allPieces);

    if (toMove == NULL)
        return NULL;

    rookMove.m = 0;

    *toMove = updatePosAndMc(*toMove, rookMove);

    return allPieces;
}

/* updates all pieces with an enpassant capture*/
Piece *captureEnPassant(Piece piece, Tuple move, Piece *allPieces)
{
    Piece *toMove;
    Tuple victimPos;

    toMove = findPiece(piece.pos, allPieces);

    if (toMove == NULL)
        return NULL;

    *toMove = updatePosAndMc(piece, move);

    victimPos.m = piece.pos.m;
    victimPos.n = piece.pos.n + move.n;

    toMove = findPiece(victimPos, allPieces);

    if (toMove == NULL)
        return NULL;

    toMove->pos.m = CAPTURED;
    toMove->pos.n = CAPTURED;

    return allPieces;
}

/* copies boardA contents onto board B */
Piece *copyBoard(Piece *boardA, Piece *boardB)
{
    int i;

    for (i = 0; i < LENGTH; i++) {
        boardB[i] = boardA[i];
    }

    return boardB;
}

/* returns whether a move will leave its king in check */
int willBeInCheck(Piece piece, Tuple move, Piece *allPieces)
{
    Piece newBoard[LENGTH];

    copyBoard(allPieces, newBoard);

    executeMove(piece, move, newBoard);

    return inCheck(piece.colour, newBoard);
}

/* executes a pawn promotion on allPieces */
Piece *promotePawn(Piece piece, Tuple move, Piece *allPieces)
{
    Tuple targetPos = getTarget(piece.pos, move);
    Piece *toMove;
    Piece *targetPiece;

    toMove = findPiece(piece.pos, allPieces);

    if (toMove == NULL)
        return NULL;

    targetPiece = findPiece(targetPos, allPieces);

    if (targetPiece != NULL) {
        targetPiece->pos.m = CAPTURED;
        targetPiece->pos.n = CAPTURED;
    }

    *toMove = updatePosAndMc(piece, move);
    toMove->typeVal = QUEEN;


    return allPieces;
}

/* returns whether this action is the last action marker */
int isLastAction(Action action)
{
    return compareTuple(action.move, LASTACTION.move) && \
        comparePieces(action.piece, LASTACTION.piece);
}

/* returns true if two pieces are the same */
int comparePieces(Piece pieceA, Piece pieceB)
{
    if (compareTuple(pieceA.pos, pieceB.pos) && \
    pieceA.colour == pieceB.colour && \
    pieceA.mc == pieceB.mc && pieceA.typeVal == pieceB.typeVal)
        return 1;

    return compareTuple(pieceA.pos, pieceB.pos) && \
        pieceA.colour == pieceB.colour && \
        pieceA.mc == pieceB.mc && pieceA.typeVal == pieceB.typeVal;

}
