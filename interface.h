#ifndef _INTERFACE
#define _INTERFACE

/*libs*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*defs*/
#define LENGTH 32
#define CAPTURED -1
#define MOVES 160 /* limit for legal move storage */

#define NULL_TUPLE { .m = CAPTURED, .m = CAPTURED }
#define NULL_PIECE { .typeVal = KING, .colour = WHITE, .mc = 0, \
    .pos = NULL_TUPLE }
#define NULL_ACTION { .move = NULL_TUPLE , .piece = NULL_PIECE }

enum colour {
    BLACK,
    WHITE
};

enum pieceType {
    KING = 1000,
    QUEEN = 900,
    ROOK = 500,
    BISHOP = 350,
    KNIGHT = 300,
    PAWN = 100
};

/*types*/
typedef struct Tuple {
    int m;
    int n;
} Tuple;

typedef struct Piece {
    int typeVal;
    int colour;
    Tuple pos;
    int mc;
} Piece;

typedef struct Action { /*pmf*/
    Piece piece;
    Tuple move;
    int eval;
} Action;

typedef struct AB {
    int alpha;
    int beta;
} AB;

typedef struct Explore {
    int noex; /* 1 or 0 for whether to explore or not */
    int eval;
} Explore;

/*consts*/
extern const Action LASTACTION;

/*util.c*/
Tuple getTarget(Tuple pos, Tuple move);
Tuple moveMade(Tuple start, Tuple end);
int compareTuple(Tuple a, Tuple b);
int furtherFromZero(int num);
int closerToZero(int num);
Piece *findPiece(Tuple pos, Piece *allPieces);
int isEmpty(Tuple pos, Piece *allPieces);
int isInBounds(int x);
int isOnBoard(Tuple pos);
int isValidTarget(Piece piece, Tuple move, Piece *allPieces);
int isDiagonalMovePathEmpty(Tuple pos, Tuple move, Piece *allPieces);
Tuple decreaseDiagonalMove(Tuple move);
Tuple decreaseStraightMove(Tuple move);
Tuple increaseStraightMove(Tuple move);
int isStraightMovePathEmpty(Tuple pos, Tuple move, Piece *allPieces);
int isStraightMove(Tuple move);
int isDiagonalMove(Tuple move);
int isLShaped(Tuple move);
int isKnightValidMove(Piece piece, Tuple move, Piece *allPieces);
int isBishopValidMove(Piece piece, Tuple move, Piece *allPieces);
int isRookValidMove(Piece piece, Tuple move, Piece *allPieces);
int isQueenValidMove(Piece piece, Tuple move, Piece *allPieces);
int pawnDirection(int colour);
int isPawnCapture(int colour, Tuple move);
int isBasicPawnMove(int colour, Tuple move);
int pawnHomeRow(int colour);
int isPawnForwardTwo(Piece pawn, Tuple move, Piece *allPieces);
int isValidEnPassant(Piece pawn, Tuple move, Piece *allPieces);
int isPawnValidMove(Piece piece, Tuple move, Piece *allPieces);
int isValidPromotion(Piece piece, Tuple move, Piece *allPieces);
int possibleToCastle(int colour, int side, Piece *allPieces);
int validCastle(Piece piece, Tuple move, Piece *allPieces);
int isThreatened(Tuple pos, int colour, Piece *allPieces);
int isValidMove(Piece piece, Tuple move, Piece *allPieces);
int isThreatened(Tuple pos, int colour, Piece *allPieces);
Piece *findKing(int colour, Piece *allPieces);
int inCheck(int colour, Piece *allPieces);
int validKingMove(Piece king, Tuple move, Piece *allPieces);
Piece updatePosAndMc(Piece piece, Tuple move);
Piece *executeMove(Piece piece, Tuple move, Piece *allPieces);
Piece *resetEnemyPawns(int colour, Piece *allPieces);
Piece *executeCastle(Piece piece, Tuple move, Piece *allPieces);
Piece *captureEnPassant(Piece piece, Tuple move, Piece *allPieces);
Piece *copyBoard(Piece *boardA, Piece *boardB);
int willBeInCheck(Piece piece, Tuple move, Piece *allPieces);
Piece *promotePawn(Piece piece, Tuple move, Piece *allPieces);
int isLastAction(Action action);
int comparePieces(Piece pieceA, Piece pieceB);
int lesser(int a, int b);

/*init.c*/
int addAllPieces(Piece *allPieces);
int addPawns(Piece *allPieces, int colour, int index);
int addTwoPieces(Piece *allPieces, int index, int column, int typeVal);

/*ui.c*/
int printBoard(Piece *allPieces);
char getPieceIcon(Piece piece);
Piece *movePrompt(Piece *allPieces);

/*eval.c*/
int totalVal(Piece *allPieces);
int countPieceType(int colour, int typeVal, Piece *allPieces);
int totalMaterial(int colour, Piece *allPieces);
int calcIndex(int colour, Tuple pos);
int evalAllPieces(int colour, Piece *allPieces);
int evalPiece(Piece piece);
int evalKnight(Piece piece);
int evalBishop(Piece piece);
int evalKing(Piece piece);
int howCentral(Tuple pos);

/*search.c*/
Action findMostEpicMove(AB ab, int depth, int colour, Piece *allPieces);
Action *addEvals(AB ab, int depth, int colour, Piece *allPieces, \
    Action *legalMoves);
Explore dontExplore(AB ab, int colour, int eval);
AB updateAB(AB ab, int colour, int eval);
Action strongestMoveFromList(int colour, Action *legalMoves);
int addAllLegalMoves(int colour, Piece *allPieces, Action *legalMoves);
int addLegalMoves(Piece piece, Piece *allPieces, Action *legalMoves,int index);
int legalPawnMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int legalKnightMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int knightMoveLegality(Piece piece, int m, int n, Piece *allPieces, \
    Action *legalMoves, int index);
int legalBishopMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int bishopLineLegality(Piece piece, int m, int n, Piece *allPieces, \
    Action *legalMoves, int index);
int legalRookMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int rookLineLegality(Piece piece, int m, int n, Piece *allPieces, \
    Action *legalMoves, int index);
int legalQueenMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int legalKingMoves(Piece piece, Piece *allPieces, \
    Action *legalMoves, int index);
int kingMoveLegality(Piece piece, int m, int n, Piece *allPieces, \
    Action *legalMoves, int index);
Action *addLastAction(Action *legalMoves, int index);

#endif
