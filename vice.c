#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

int main()
{

  AllInit();
 
  
  int PieceOne = rand();
  int PieceTwo = rand();
  int PieceThree = rand();
  int PieceFour = rand();

  printf("PieceOne:%X\n", PieceOne);
  printf("PieceTwo:%X\n", PieceTwo);
  printf("PieceThree:%X\n", PieceThree);
  printf("PieceFour:%X\n", PieceFour);

  int Key = PieceOne ^ PieceTwo ^ PieceThree ^ PieceFour;
  int TempKey = PieceOne;
  TempKey ^= PieceTwo;
  TempKey ^= PieceThree;
  TempKey ^= PieceFour;

  printf("Key:%X\n", Key);
  printf("TempKey:%X\n", TempKey);


  return 0;
}