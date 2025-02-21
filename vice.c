#include "stdio.h"
#include "defs.h"

int main()
{

  AllInit();
  int index = 0;

  U64 playBitBoard = 0ULL;

  playBitBoard |= (1ULL << SQ64(D2));
  playBitBoard |= (1ULL << SQ64(D3));
  playBitBoard |= (1ULL << SQ64(D4));

  for (index = 0; index < 64; ++index)
  {
    printf("Index:%d\n", index);
    PrintBitBoard(SetMask[index]);
    printf("\n");
  }


  return 0;
}