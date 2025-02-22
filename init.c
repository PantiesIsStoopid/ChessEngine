// init.c

#include "defs.h"
#include "stdio.h"
#include "stdlib.h"

// Macro to generate a random 64-bit number using rand()
#define RAND_64 ((U64)rand() |       \
                 (U64)rand()         \
                     << 15 |         \
                 (U64)rand()         \
                     << 30 |         \
                 (U64)rand()         \
                     << 45 |         \
                 ((U64)rand() & 0xf) \
                     << 60)

// Global arrays for board representation and hash keys
int Sq120ToSq64[BRD_SQ_NUM];  // Mapping from 120-square board to 64-square board
int Sq64ToSq120[64];           // Mapping from 64-square board to 120-square board

U64 SetMask[64];               // Masks for setting specific bits in a bitboard
U64 ClearMask[64];             // Masks for clearing specific bits in a bitboard

U64 PieceKeys[13][120];        // Hash keys for pieces
U64 SideKey;                   // Hash key for side to move
U64 CastleKeys[16];            // Hash keys for castling permissions

int FilesBrd[BRD_SQ_NUM];      // Array to store file (column) of each square
int RanksBrd[BRD_SQ_NUM];      // Array to store rank (row) of each square

// Function to initialize the file and rank board arrays
void InitFilesRanksBrd()
{
  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int sq = A1;

  // Set all board squares to OFFBOARD initially
  for (index = 0; index < BRD_SQ_NUM; ++index)
  {
    FilesBrd[index] = OFFBOARD;
    RanksBrd[index] = OFFBOARD;
  }

  // Map the correct file and rank for each square on the board
  for (rank = RANK_1; rank <= RANK_8; ++rank)
  {
    for (file = FILE_A; file <= FILE_H; ++file)
    {
      sq = FR2SQ(file, rank);
      FilesBrd[sq] = file;
      RanksBrd[sq] = rank;
    }
  }
}

// Function to initialize the hash keys for pieces, side, and castling
void InitHashKeys()
{
  int index = 0;
  int index2 = 0;

  // Assign random hash keys for each piece type and square
  for (index = 0; index < 13; ++index)
  {
    for (index2 = 0; index2 < 120; ++index2)
    {
      PieceKeys[index][index2] = RAND_64;
    }
  }

  // Generate random keys for side and castling permissions
  SideKey = RAND_64;
  for (index = 0; index < 16; ++index)
  {
    CastleKeys[index] = RAND_64;
  }
}

// Function to initialize the bit masks for each square
void InitBitMasks()
{
  int index = 0;

  // Set all masks to 0 initially
  for (index = 0; index < 64; ++index)
  {
    SetMask[index] = 0ULL;
    ClearMask[index] = 0ULL;
  }

  // Set the SetMask to have a bit set at each square position
  for (index = 0; index < 64; ++index)
  {
    SetMask[index] |= (1ULL << index);   // Set the bit at the index
    ClearMask[index] = ~SetMask[index];  // Clear the bit at the index
  }
}

// Function to initialize the 120-square and 64-square board mappings
void InitSq120To64()
{
  int index = 0;
  int file = FILE_A;
  int rank = RANK_1;
  int sq = A1;
  int sq64 = 0;

  // Set all squares to invalid values initially
  for (index = 0; index < BRD_SQ_NUM; ++index)
  {
    Sq120ToSq64[index] = 65;  // Invalid square
  }

  for (index = 0; index < 64; ++index)
  {
    Sq64ToSq120[index] = 120; // Invalid square
  }

  // Fill the mappings for valid squares on the board
  for (rank = RANK_1; rank <= RANK_8; ++rank)
  {
    for (file = FILE_A; file <= FILE_H; ++file)
    {
      sq = FR2SQ(file, rank);
      Sq64ToSq120[sq64] = sq;
      Sq120ToSq64[sq] = sq64;
      sq64++;
    }
  }
}

// Function to initialize all data structures
void AllInit()
{
  InitSq120To64();
  InitBitMasks();
  InitHashKeys();
  InitFilesRanksBrd();
}
