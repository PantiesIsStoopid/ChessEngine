// data.c

#include "defs.h"

// Character representations of pieces (capital for white, lowercase for black)
char PceChar[] = ".PNBRQKpnbrqk";

// Character representations of side (white, black, or neither for no side)
char SideChar[] = "wb-";

// Character representations of ranks (1 to 8)
char RankChar[] = "12345678";

// Character representations of files (a to h)
char FileChar[] = "abcdefgh";

// Array indicating if a piece is big (Rooks, Bishops, Queens, Kings)
int PieceBig[13] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE};

// Array indicating if a piece is major (Rooks, Bishops, Queens, Kings)
int PieceMaj[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE};

// Array indicating if a piece is minor (Knights, Bishops)
int PieceMin[13] = {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE};

// Array indicating the value of each piece
int PieceVal[13] = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};

// Array indicating the color of each piece (WHITE, BLACK, BOTH for no side)
int PieceCol[13] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

// Array indicating if a piece is a Knight
int PieceKnight[13] = {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE};

// Array indicating if a piece is a King
int PieceKing[13] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE};

// Array indicating if a piece is a Rook or Queen
int PieceRookQueen[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE};

// Array indicating if a piece is a Bishop or Queen
int PieceBishopQueen[13] = {FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE};

// Array indicating if a piece moves by sliding (Bishops, Rooks, Queens)
int PieceSlides[13] = {FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE};
