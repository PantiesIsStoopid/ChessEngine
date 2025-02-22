// io.c

#include "stdio.h"
#include "defs.h"

// Function to print a square in algebraic notation (e.g., 'a1', 'h8')
char *PrSq(const int sq)
{
  static char sqStr[3];  // Static buffer to hold the square string (e.g., "a1")

  // Get the file and rank of the square
  int file = FilesBrd[sq];
  int rank = RanksBrd[sq];

  // Format the square string as 'a1', 'h8', etc.
  sprintf(sqStr, "%c%c", ('a' + file), ('1' + rank));

  return sqStr;  // Return the formatted string
}

// Function to print a move in algebraic notation (e.g., 'e2e4', 'e7e8q')
char *PrMove(const int move)
{
  static char MvStr[6];  // Static buffer to hold the move string (e.g., "e2e4")

  // Get the file and rank of the source and destination squares
  int ff = FilesBrd[FROMSQ(move)];
  int rf = RanksBrd[FROMSQ(move)];
  int ft = FilesBrd[TOSQ(move)];
  int rt = RanksBrd[TOSQ(move)];

  // Check if the move is a promotion
  int promoted = PROMOTED(move);

  // If it's a promotion, append the promotion piece (e.g., 'q', 'n', 'r', 'b')
  if (promoted)
  {
    char pchar = 'q';  // Default to Queen promotion

    // Check the type of promotion and set the corresponding character
    if (IsKn(promoted)) {
      pchar = 'n';  // Knight promotion
    } else if (IsRQ(promoted) && !IsBQ(promoted)) {
      pchar = 'r';  // Rook promotion
    } else if (!IsRQ(promoted) && IsBQ(promoted)) {
      pchar = 'b';  // Bishop promotion
    }

    // Format the move with promotion (e.g., 'e7e8q')
    sprintf(MvStr, "%c%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt), pchar);
  }
  else
  {
    // Format the move without promotion (e.g., 'e2e4')
    sprintf(MvStr, "%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt));
  }

  return MvStr;  // Return the formatted move string
}
