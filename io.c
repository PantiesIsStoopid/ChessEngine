#include "stdio.h"
#include "defs.h"

char *PrSq(const int sq)
{
  static char sqStr[3];

  int file = FilesBrd[sq];
  int rank = RanksBrd[sq];

  sprintf(sqStr, "%c%c", ('a' + file), ('1' + rank));

  return sqStr;
}

char *PrMove(const int move) {
  static char MvStr[6];

  int ff = FilesBrd[FROMSQ(move)];
  int rf = RanksBrd[FROMSQ(move)];
  int ft = FilesBrd[TOSQ(move)];
  int rt = RanksBrd[TOSQ(move)];

  int promoted = PROMOTED(move);

  if (promoted) {
    char pchar = 'q';
    if (IsKn(promoted)) {
      pchar = 'n';
    } else if (IsRQ(promoted) && !IsBQ(promoted)) {
      pchar = 'r';
    } else if (!IsRQ(promoted) && IsBQ(promoted)) {
      pchar = 'b';
    }
    sprintf(MvStr, "%c%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt), pchar);
  } else {
    sprintf(MvStr, "%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt));
  }
  return MvStr;
}