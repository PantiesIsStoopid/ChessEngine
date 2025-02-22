// vice.c

#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

// FEN string for a sample position (can be used for initialization)
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NQ1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{
  // Initialize all necessary data structures
  AllInit();

  // Example move data
  int move = 0;
  int from = A2;  // Source square (A2)
  int to = H7;    // Destination square (H7)
  int cap = wR;   // Captured piece (White Rook)
  int prom = wR;  // Promotion piece (White Rook)

  // Construct the move encoding the source, destination, capture, and promotion
  move = ((from | (to << 7) | (cap << 14) | (prom << 20)));

  // Print the details of the move
  printf("from:%d to:%d cap:%d prom:%d\n",
         FROMSQ(move), TOSQ(move), CAPTURED(move),
         PROMOTED(move));

  // Print the square from which the move is made
  printf("from: %s\n", PrSq(from));
  // Print the destination square of the move
  printf("to: %s\n", PrSq(to));
  // Print the move in standard algebraic notation
  printf("move: %s\n", PrMove(move));

  return 0;  // Return success
}
