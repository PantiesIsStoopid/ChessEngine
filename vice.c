#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NQ1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{

  AllInit();

  S_BOARD board[1];

  ParseFen(FEN4, board);

  PrintBoard(board);

  ASSERT(CheckBoard(board));

  int move = 0;
  int from = 6;
  int to = 12;
  int cap = wR;
  int prom = bR;
  move = ((from) | (to << 7) | (cap << 14) | (prom << 20));
  
  printf("Move: %d\n", move);
  return 0;
}