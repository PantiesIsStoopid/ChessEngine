// movegen.c

#include "stdio.h"
#include "defs.h"

// Macro to generate a move with all necessary parameters
#define MOVE(f, t, ca, pro, fl) ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
// Macro to check if a square is off the board
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

// Function to add a quiet move (non-capturing) to the move list
void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  // Store the move in the move list at the current count position
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0; // Set the move score to 0 (no capture)
  list->count++;                      // Increment the move count
}

// Function to add a capture move to the move list
void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  // Store the capture move in the move list at the current count position
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0; // Set the move score to 0 (for simplicity)
  list->count++;                      // Increment the move count
}

// Function to add an en passant move to the move list
void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
  // Store the en passant move in the move list at the current count position
  list->moves[list->count].move = move;
  list->moves[list->count].score = 0;
  list->count++; // Increment the move count
}

// Function to add white pawn capture move to the move list
void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list)
{
  // If the white pawn is on the 7th rank, promote the capture to a queen, rook, bishop, or knight
  if (RanksBrd[from] == RANK_7)
  {
    AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
  }
  else
  {
    // Otherwise, just add a regular capture move
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

// Function to add white pawn move to the move list
void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list)
{
  ASSERT(SqOnBoard(from));   // Ensure the 'from' square is on the board
  ASSERT(SqOnBoard(to));     // Ensure the 'to' square is on the board
  ASSERT(CheckBoard(pos));   // Ensure the board is in a valid state

  // If the white pawn is on the 7th rank, promote the move to a queen, rook, bishop, or knight
  if (RanksBrd[from] == RANK_7)
  {
    AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  }
  else
  {
    // Otherwise, just add a regular quiet move
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

// Function to add black pawn capture move to the move list
void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list)
{
  // If the black pawn is on the 2nd rank, promote the capture to a queen, rook, bishop, or knight
  if (RanksBrd[from] == RANK_2)
  {
    AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bN, 0), list);
  }
  else
  {
    // Otherwise, just add a regular capture move
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

// Function to add black pawn move to the move list
void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list)
{
  ASSERT(SqOnBoard(from));   // Ensure the 'from' square is on the board
  ASSERT(SqOnBoard(to));     // Ensure the 'to' square is on the board
  ASSERT(CheckBoard(pos));   // Ensure the board is in a valid state

  // If the black pawn is on the 7th rank, promote the move to a queen, rook, bishop, or knight
  if (RanksBrd[from] == RANK_7)
  {
    AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);
  }
  else
  {
    // Otherwise, just add a regular quiet move
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

// Function to generate all possible moves for the current side
void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list)
{
  ASSERT(CheckBoard(pos));  // Ensure the board is in a valid state

  list->count = 0;  // Initialize move count

  int pce = EMPTY;
  int side = pos->side;
  int sq = 0;
  int t_sq = 0;
  int pceNum = 0;

  // Loop through all pieces based on the current side (White or Black)
  if (side == WHITE)
  {
    // Generate moves for white pawns
    for (pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum)
    {
      sq = pos->pList[wP][pceNum];
      ASSERT(SqOnBoard(sq));

      // Check if the square in front of the pawn is empty
      if (pos->pieces[sq + 10] == EMPTY)
      {
        AddWhitePawnMove(pos, sq, sq + 10, list);

        // Check if the pawn is on the 2nd rank and if the next two squares are empty
        if (RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY)
        {
          AddQuietMove(pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      // Check if a capture move is possible
      if (!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK)
      {
        AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
      }
      if (!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK)
      {
        AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);
      }

      // Check if en passant is available
      if (pos->enPas != NO_SQ)
      {
        if (sq + 9 == pos->enPas)
        {
          AddEnPassantMove(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
        }
        if (sq + 11 == pos->enPas)
        {
          AddEnPassantMove(pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
        }
      }
    }
  }
  else
  {
    // Generate moves for black pawns
    for (pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum)
    {
      sq = pos->pList[bP][pceNum];
      ASSERT(SqOnBoard(sq));

      // Check if the square in front of the pawn is empty
      if (pos->pieces[sq - 10] == EMPTY)
      {
        AddBlackPawnMove(pos, sq, sq - 10, list);

        // Check if the pawn is on the 7th rank and if the next two squares are empty
        if (RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY)
        {
          AddQuietMove(pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      // Check if a capture move is possible
      if (!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE)
      {
        AddBlackPawnCapMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
      }
      if (!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE)
      {
        AddBlackPawnCapMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);
      }

      // Check if en passant is available
      if (pos->enPas != NO_SQ)
      {
        if (sq - 9 == pos->enPas)
        {
          AddEnPassantMove(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
        }
        if (sq - 11 == pos->enPas)
        {
          AddEnPassantMove(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
        }
      }
    }
  }
}
