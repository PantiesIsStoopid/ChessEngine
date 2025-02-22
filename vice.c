// vice.c

#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

// FEN string for a sample position (can be used for initialization)
#define PAWNMOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"


int main()
{
  // Initialize all necessary data structures
  AllInit();

  S_BOARD board[1]; // Create a board structure
  S_MOVELIST list[1]; // Create a move list structure

  ParseFen(PAWNMOVES, board); // Parse the sample FEN string
  GenerateAllMoves(board, list); // Generate all possible moves

  return 0; // Return success
}
