// hashkeys.c

#include <stdio.h>
#include "defs.h"

// Function to generate a unique position key based on the current board state
U64 GeneratePosKey(const S_BOARD *pos)
{
  int sq = 0;
  U64 finalKey = 0;
  int piece = EMPTY;

  // Loop through all squares on the board
  for (sq = 0; sq < BRD_SQ_NUM; ++sq)
  {
    piece = pos->pieces[sq];
    
    // If the square is not empty or off the board, apply the corresponding piece key
    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD)
    {
      ASSERT(piece >= wP && piece <= bK);  // Assert that the piece is valid
      finalKey ^= PieceKeys[piece][sq];    // XOR the piece's key with the final key
    }
  }

  // If it's white's turn, apply the side key
  if (pos->side == WHITE)
  {
    finalKey ^= SideKey;
  }

  // If there's an en passant square, XOR the corresponding piece key
  if (pos->enPas != NO_SQ)
  {
    ASSERT(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM);  // Assert that en passant square is valid
    finalKey ^= PieceKeys[EMPTY][pos->enPas];  // XOR the empty piece key for en passant square
  }

  // Assert that the castling permissions are valid
  ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

  // XOR the castling keys based on the permissions
  finalKey ^= CastleKeys[pos->castlePerm];

  // Return the final position key
  return finalKey;
}
