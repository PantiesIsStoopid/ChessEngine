// defs.h

#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"

// Enable debugging
#define DEBUG

// Assert macro used for debugging
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                     \
  if (!(n))                           \
  {                                   \
    printf("%s - Failed", #n);        \
    printf("On %s ", __DATE__);       \
    printf("At %s ", __TIME__);       \
    printf("In File %s ", __FILE__);  \
    printf("At Line %d\n", __LINE__); \
    exit(1);                          \
  }
#endif

// Type definitions
typedef unsigned long long U64; // 64-bit unsigned integer

// Constants and board dimensions
#define NAME "Vice 1.0"
#define BRD_SQ_NUM 120 // Total number of squares on the board

#define MAXGAMEMOVES 2048    // Maximum number of game moves
#define MAXPOSITIONMOVES 256 // Maximum number of moves for a position

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" // Initial FEN string

// Piece enumeration
// clang-format off
enum
{
  EMPTY,
  wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK
};

// File enumeration (A-H, or NONE)
enum
{
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE
};

// Rank enumeration (1-8, or NONE)
enum
{
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE
};

// Side enumeration (White, Black, or Both)
enum
{
  WHITE, BLACK, BOTH
};

// Board square coordinates (using a 10x12 board)
enum
{
  A1 = 21,  B1,  C1,  D1,  E1,  F1,  G1,  H1,
  A2 = 31,  B2,  C2,  D2,  E2,  F2,  G2,  H2,
  A3 = 41,  B3,  C3,  D3,  E3,  F3,  G3,  H3,
  A4 = 51,  B4,  C4,  D4,  E4,  F4,  G4,  H4,
  A5 = 61,  B5,  C5,  D5,  E5,  F5,  G5,  H5,
  A6 = 71,  B6,  C6,  D6,  E6,  F6,  G6,  H6,
  A7 = 81,  B7,  C7,  D7,  E7,  F7,  G7,  H7,
  A8 = 91,  B8,  C8,  D8,  E8,  F8,  G8,  H8,
  NO_SQ, OFFBOARD
};

// clang-format on

// Boolean values
enum
{
  FALSE,
  TRUE
};

// Castling permissions
enum
{
  WKCA = 1,
  WQCA = 2,
  BKCA = 4,
  BQCA = 8
};

// Structure to store game move with its associated score
typedef struct
{
  int move;
  int score;
} S_MOVE;

typedef struct
{
  S_MOVE moves[MAXPOSITIONMOVES];
  int count;
} S_MOVELIST;

// Structure to store undo information (move, castling permissions, en passant, etc.)
typedef struct
{
  int move;
  int castlePerm;
  int enPas;
  int fiftyMove;
  U64 posKey;
} S_UNDO;

// Board structure
typedef struct
{
  int pieces[BRD_SQ_NUM]; // Array of pieces on the board
  U64 pawns[3];           // Bitboards for white, black, and both pawns

  int kingSq[2]; // Positions of white and black kings

  int side;      // Side to move (0 for white, 1 for black)
  int enPas;     // En passant square
  int fiftyMove; // Fifty-move rule counter

  int ply;    // Half move counter
  int hisPly; // Full move counter

  int castlePerm; // Castling permissions (4 bits for each side)

  U64 posKey; // Position key for hashing

  int pceNum[13];  // Number of pieces of each type
  int bigPce[2];   // Number of non-pawn pieces (big pieces)
  int majPce[2];   // Number of major pieces (rooks, queens)
  int minPce[2];   // Number of minor pieces (knights, bishops)
  int material[2]; // Material score for both sides

  S_UNDO history[MAXGAMEMOVES]; // History of moves for undo functionality

  int pList[13][10]; // Piece lists (for each type)
} S_BOARD;

// Game move bit encoding:
// These macros help in extracting the relevant information from a move representation
#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m) >> 7) & 0x7F)
#define CAPTURED(m) (((m) >> 14) & 0xF)
#define PROMOTED(m) (((m) >> 20) & 0xF)

// Move flags
#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

// Macros for bit manipulation and square conversion
#define FR2SQ(f, r) ((21 + (f)) + ((r) * 10))
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
#define SQ120(sq64) (Sq64ToSq120[(sq64)])
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

// Piece classification checks
#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

// External global variables
extern int Sq120ToSq64[BRD_SQ_NUM]; // Square conversion table from 120 to 64 squares
extern int Sq64ToSq120[64];         // Square conversion table from 64 to 120 squares
extern U64 SetMask[64];             // Masks for setting bits
extern U64 ClearMask[64];           // Masks for clearing bits

extern U64 PieceKeys[13][120]; // Piece key hashes for each piece
extern U64 SideKey;            // Side key for hash table
extern U64 CastleKeys[16];     // Castling keys for hash table

extern char PceChar[];  // Piece character representation
extern char SideChar[]; // Side character representation
extern char RankChar[]; // Rank character representation
extern char FileChar[]; // File character representation

extern int PieceBig[13]; // Array for big pieces
extern int PieceMaj[13]; // Array for major pieces
extern int PieceMin[13]; // Array for minor pieces
extern int PieceVal[13]; // Piece values
extern int PieceCol[13]; // Piece color array

extern int FilesBrd[BRD_SQ_NUM]; // Files for each square on the board
extern int RanksBrd[BRD_SQ_NUM]; // Ranks for each square on the board

extern int PieceKnight[13];      // Knight piece identification array
extern int PieceKing[13];        // King piece identification array
extern int PieceRookQueen[13];   // Rook and Queen identification array
extern int PieceBishopQueen[13]; // Bishop and Queen identification array
extern int PieceSlides[13];      // Sliding piece identification array

// External functions

// init.c
extern void AllInit(); // Initialize all components

// bitboards.c
extern void PrintBitBoard(U64 bb); // Print a bitboard
extern int PopBit(U64 *bb);        // Pop a bit from a bitboard
extern int CountBits(U64 b);       // Count the number of bits set in a bitboard

// hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos); // Generate a position key for hashing

// board.c
extern void ResetBoard(S_BOARD *pos);          // Reset the board to the starting position
extern int ParseFen(char *fen, S_BOARD *pos);  // Parse a FEN string
extern void PrintBoard(const S_BOARD *pos);    // Print the board
extern void UpdateListsMaterial(S_BOARD *pos); // Update piece lists and material counts
extern int CheckBoard(const S_BOARD *pos);     // Check the validity of the board position

// attack.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos); // Check if square is attacked

// io.c
extern char *PrSq(const int sq);     // Print square
extern char *PrMove(const int move); // Print move
extern void PrintMoveList(const S_MOVELIST *list); // Print move list

// validate.c
extern int SqOnBoard(const int sq);        // Check if square is on the board
extern int SideValid(const int side);      // Check if side is valid
extern int FileRankValid(const int fr);    // Check if file and rank are valid
extern int PieceValidEmpty(const int pce); // Check if piece is valid and not empty
extern int PieceValid(const int pce);      // Check if piece is valid

// movegen.c
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list); // Generate all possible moves

#endif // DEFS_H
