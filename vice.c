#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NQ1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{

  AllInit();

  int move = 0;
  int from = A2;
  int to = H7;
  int cap = wR;
  int prom = wR;

  move = ((from | (to << 7) | (cap << 14) | (prom << 20)));

  printf("from:%d to:%d cap:%d prom:%d\n",
         FROMSQ(move), TOSQ(move), CAPTURED(move),
         PROMOTED(move));

  printf("from: %s\n", PrSq(from));
  printf("to: %s\n", PrSq(to));
  printf("move: %s\n", PrMove(move));
  return 0;
}