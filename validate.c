// validate.c

#include "defs.h"

// Function to check if a square is on the board
int SqOnBoard(const int sq)
{
  // Returns 1 if the square is on the board, 0 if off the board
  return FilesBrd[sq] == OFFBOARD ? 0 : 1;
}

// Function to check if the side is valid (WHITE or BLACK)
int SideValid(const int side)
{
  // Returns 1 if the side is valid, otherwise 0
  return side == WHITE || side == BLACK ? 1 : 0;
}

// Function to check if a file-rank combination is valid
int FileRankValid(const int fr)
{
  const int file = fr % 10; // Extract the file (last digit)
  const int rank = fr / 10; // Extract the rank (first digit)

  // Returns 1 if file and rank are within valid range, otherwise 0
  return file >= FILE_A && file <= FILE_H && rank >= RANK_1 && rank <= RANK_8 ? 1 : 0;
}

// Function to check if a piece is valid or if it's empty
int PieceValidEmpty(const int pce)
{
  // Returns 1 if the piece is either empty or valid (wP to bK), otherwise 0
  return pce >= EMPTY && pce <= bK ? 1 : 0;
}

// Function to check if a piece is valid (wP to bK)
int PieceValid(const int pce)
{
  // Returns 1 if the piece is valid (wP to bK), otherwise 0
  return pce >= wP && pce <= bK ? 1 : 0;
}
