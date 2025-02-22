#include "stdio.h"
#include "defs.h"

// Macro to generate a move with all necessary parameters
#define MOVE(f, t, ca, pro, fl) ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
// Macro to check if a square is off the board
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

// Arrays for different piece types and their movement logic
int LoopSlidePce[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int LoopNonSlidePce[6] = {wN, wK, 0, bN, bK, 0};
int LoopSlideIndex[2] = {0, 4};
int LoopNonSlideIndex[2] = {0, 3};

// Function to add a quiet move (non-capturing) to the move list
void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0; // No capture, so score is 0
  list->count++; // Increment move count
}

// Function to add a capture move to the move list
void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0; // Capture move but score not assigned here
  list->count++;
}

// Function to add an en passant move to the move list
void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++;
}

// Function to add a white pawn capture move to the move list
void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list)
{
  ASSERT(PieceValidEmpty(cap));
  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  // If the pawn is on rank 7, promotion occurs
  if (RanksBrd[from] == RANK_7)
  {
    AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
  }
  else
  {
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

// Function to add a white pawn move (non-capturing) to the move list
void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list)
{
  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  // Promote if on rank 7
  if (RanksBrd[from] == RANK_7)
  {
    AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  }
  else
  {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

// Function to generate all possible moves for the current side
void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list)
{
  ASSERT(CheckBoard(pos));
  list->count = 0;
  int pce, side, sq, t_sq, pceNum, dir, index, pceIndex;
  side = pos->side;

  printf("Side:%c\n", SideChar[side]);

  // Generate pawn moves for the current side
  if (side == WHITE)
  {
    for (pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum)
    {
      sq = pos->pList[wP][pceNum];
      ASSERT(SqOnBoard(sq));

      // Check if single or double move forward is possible
      if (pos->pieces[sq + 10] == EMPTY)
      {
        AddWhitePawnMove(pos, sq, sq + 10, list);
        if (RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY)
        {
          AddQuietMove(pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      // Check for captures and en passant
      if (!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK)
      {
        AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
      }
      if (!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK)
      {
        AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);
      }
    }
  }
  else
  {
    for (pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum)
    {
      sq = pos->pList[bP][pceNum];
      ASSERT(SqOnBoard(sq));

      // Check if single or double move forward is possible
      if (pos->pieces[sq - 10] == EMPTY)
      {
        AddBlackPawnMove(pos, sq, sq - 10, list);
        if (RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY)
        {
          AddQuietMove(pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAGPS), list);
        }
      }
    }
  }

  // Loop through sliding pieces
  pceIndex = LoopSlideIndex[side];
  pce = LoopSlidePce[pceIndex++];
  while (pce != 0)
  {
    ASSERT(PieceValid(pce));
    printf("sliding piece index:%d pce:%c\n", pceIndex, PceChar[pce]);
    pce = LoopSlidePce[pceIndex++];
  }

  // Loop through non-sliding pieces
  pceIndex = LoopNonSlideIndex[side];
  pce = LoopNonSlidePce[pceIndex++];
  while (pce != 0)
  {
    ASSERT(PieceValid(pce));
    printf("sliding piece index:%d pce:%c\n", pceIndex, PceChar[pce]);
    pce = LoopNonSlidePce[pceIndex++];
  }
}
