// board.c

#include "stdio.h"
#include "defs.h"

// Check the integrity of the board state and piece information
int CheckBoard(const S_BOARD *pos)
{
  // Initialize arrays for piece counts and material tracking
  int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int t_bigPce[2] = {0, 0};
  int t_majPce[2] = {0, 0};
  int t_minPce[2] = {0, 0};
  int t_material[2] = {0, 0};

  int sq64, t_piece, t_pce_num, sq120, colour, pcount;

  U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

  // Set bitboards for pawns of both sides
  t_pawns[WHITE] = pos->pawns[WHITE];
  t_pawns[BLACK] = pos->pawns[BLACK];
  t_pawns[BOTH] = pos->pawns[BOTH];

  // Check piece lists for integrity
  for (t_piece = wP; t_piece <= bK; ++t_piece)
  {
    for (t_pce_num = 0; t_pce_num < pos->pceNum[t_piece]; ++t_pce_num)
    {
      sq120 = pos->pList[t_piece][t_pce_num];
      ASSERT(pos->pieces[sq120] == t_piece);
    }
  }

  // Check piece counts and material values
  for (sq64 = 0; sq64 < 64; ++sq64)
  {
    sq120 = SQ120(sq64);
    t_piece = pos->pieces[sq120];
    t_pceNum[t_piece]++;
    colour = PieceCol[t_piece];
    if (PieceBig[t_piece] == TRUE)
      t_bigPce[colour]++;
    if (PieceMin[t_piece] == TRUE)
      t_minPce[colour]++;
    if (PieceMaj[t_piece] == TRUE)
      t_majPce[colour]++;

    t_material[colour] += PieceVal[t_piece];
  }

  // Verify piece counts match expectations
  for (t_piece = wP; t_piece <= bK; ++t_piece)
  {
    ASSERT(t_pceNum[t_piece] == pos->pceNum[t_piece]);
  }

  // Check bitboard counts and verify pawn positions
  pcount = CNT(t_pawns[WHITE]);
  ASSERT(pcount == pos->pceNum[wP]);
  pcount = CNT(t_pawns[BLACK]);
  ASSERT(pcount == pos->pceNum[bP]);
  pcount = CNT(t_pawns[BOTH]);
  ASSERT(pcount == (pos->pceNum[bP] + pos->pceNum[wP]));

  // Verify bitboard squares match piece positions
  while (t_pawns[WHITE])
  {
    sq64 = POP(&t_pawns[WHITE]);
    ASSERT(pos->pieces[SQ120(sq64)] == wP);
  }

  while (t_pawns[BLACK])
  {
    sq64 = POP(&t_pawns[BLACK]);
    ASSERT(pos->pieces[SQ120(sq64)] == bP);
  }

  while (t_pawns[BOTH])
  {
    sq64 = POP(&t_pawns[BOTH]);
    ASSERT((pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP));
  }

  // Verify material, piece counts, and positions
  ASSERT(t_material[WHITE] == pos->material[WHITE] && t_material[BLACK] == pos->material[BLACK]);
  ASSERT(t_minPce[WHITE] == pos->minPce[WHITE] && t_minPce[BLACK] == pos->minPce[BLACK]);
  ASSERT(t_majPce[WHITE] == pos->majPce[WHITE] && t_majPce[BLACK] == pos->majPce[BLACK]);
  ASSERT(t_bigPce[WHITE] == pos->bigPce[WHITE] && t_bigPce[BLACK] == pos->bigPce[BLACK]);

  // Ensure the side and position key are valid
  ASSERT(pos->side == WHITE || pos->side == BLACK);
  ASSERT(GeneratePosKey(pos) == pos->posKey);

  // Ensure en passant is valid for the current position
  ASSERT(pos->enPas == NO_SQ || (RanksBrd[pos->enPas] == RANK_6 && pos->side == WHITE) || (RanksBrd[pos->enPas] == RANK_3 && pos->side == BLACK));

  // Ensure the kings are in the correct positions
  ASSERT(pos->pieces[pos->kingSq[WHITE]] == wK);
  ASSERT(pos->pieces[pos->kingSq[BLACK]] == bK);

  return TRUE;
}

// Update material and piece list for the board
void UpdateListsMaterial(S_BOARD *pos)
{
  int piece, sq, index, colour;

  // Loop through all the squares and update the board's pieces and material values
  for (index = 0; index < BRD_SQ_NUM; ++index)
  {
    sq = index;
    piece = pos->pieces[index];
    if (piece != OFFBOARD && piece != EMPTY)
    {
      // clang-format off
      colour = PieceCol[piece];

      // Update piece counts for material categories
      if (PieceBig[piece] == TRUE) pos->bigPce[colour]++;
      if (PieceMin[piece] == TRUE) pos->minPce[colour]++;
      if (PieceMaj[piece] == TRUE) pos->majPce[colour]++;
      // clang-format on

      pos->material[colour] += PieceVal[piece];

      pos->pList[piece][pos->pceNum[piece]] = sq;
      pos->pceNum[piece]++;

      // Track kings' positions
      if (piece == wK)
      {
        pos->kingSq[WHITE] = sq;
      }
      if (piece == bK)
      {
        pos->kingSq[BLACK] = sq;
      }

      // Update pawn bitboards
      if (piece == wP)
      {
        SETBIT(pos->pawns[WHITE], SQ64(sq));
        SETBIT(pos->pawns[BOTH], SQ64(sq));
      }
      else if (piece == bP)
      {
        SETBIT(pos->pawns[BLACK], SQ64(sq));
        SETBIT(pos->pawns[BOTH], SQ64(sq));
      }
    }
  }
}

// Parse the FEN string and update the board position accordingly
int ParseFen(char *fen, S_BOARD *pos)
{
  ASSERT(fen != NULL);
  ASSERT(pos != NULL);

  int rank = RANK_8;
  int file = FILE_A;
  int piece = 0;
  int count = 0;
  int i = 0;
  int sq64 = 0;
  int sq120 = 0;

  ResetBoard(pos);

  while ((rank >= RANK_1) && *fen)
  {
    count = 1;
    switch (*fen)
    {
    case 'p':
      piece = bP;
      break;
    case 'r':
      piece = bR;
      break;
    case 'n':
      piece = bN;
      break;
    case 'b':
      piece = bB;
      break;
    case 'k':
      piece = bK;
      break;
    case 'q':
      piece = bQ;
      break;
    case 'P':
      piece = wP;
      break;
    case 'R':
      piece = wR;
      break;
    case 'N':
      piece = wN;
      break;
    case 'B':
      piece = wB;
      break;
    case 'K':
      piece = wK;
      break;
    case 'Q':
      piece = wQ;
      break;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      piece = EMPTY;
      count = *fen - '0';
      break;

    case '/':
    case ' ':
      rank--;
      file = FILE_A;
      fen++;
      continue;

    default:
      printf("FEN error \n");
      return -1;
    }

    // Set pieces on the board according to the FEN string
    for (i = 0; i < count; i++)
    {
      sq64 = rank * 8 + file;
      sq120 = SQ120(sq64);
      if (piece != EMPTY)
      {
        pos->pieces[sq120] = piece;
      }
      file++;
    }
    fen++;
  }

  ASSERT(*fen == 'w' || *fen == 'b');

  pos->side = (*fen == 'w') ? WHITE : BLACK;
  fen += 2;

  // clang-format off
  // Handle castling permissions


  for (i = 0; i < 4; i++) {
    if (*fen == ' ') {
        break;
    }
  switch(*fen) {
    case 'K': pos->castlePerm |= WKCA; break;
    case 'Q': pos->castlePerm |= WQCA; break;
    case 'k': pos->castlePerm |= BKCA; break;
    case 'q': pos->castlePerm |= BQCA; break;
    default:	     break;
      }
    fen++;
  }
  fen++;
  // clang-format on

  // Ensure that the castle permissions are within a valid range (0 to 15)
  ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

  // If en passant is not represented by '-', then parse the en passant square
  if (*fen != '-')
  {
    // Convert the FEN file (a-h) to a numeric value
    file = fen[0] - 'a';
    // Convert the FEN rank (1-8) to a numeric value
    rank = fen[1] - '1';

    // Ensure the file is within valid bounds (FILE_A to FILE_H)
    ASSERT(file >= FILE_A && file <= FILE_H);
    // Ensure the rank is within valid bounds (RANK_1 to RANK_8)
    ASSERT(rank >= RANK_1 && rank <= RANK_8);

    // Set the en passant square using the calculated file and rank
    pos->enPas = FR2SQ(file, rank);
  }

  // Generate the position key for the current board state
  pos->posKey = GeneratePosKey(pos);

  // Update material lists and piece information
  UpdateListsMaterial(pos);

  return 0;
}

// Reset the board to its initial empty state
void ResetBoard(S_BOARD *pos)
{
  int index = 0;

  // Set all squares to OFFBOARD (invalid squares)
  for (index = 0; index < BRD_SQ_NUM; ++index)
  {
    pos->pieces[index] = OFFBOARD;
  }

  // Set all on-board squares to EMPTY (no pieces)
  for (index = 0; index < 64; ++index)
  {
    pos->pieces[SQ120(index)] = EMPTY;
  }

  // Reset piece counts, bitboards, and other state variables
  for (index = 0; index < 3; ++index)
  {
    pos->bigPce[index] = 0;
    pos->majPce[index] = 0;
    pos->minPce[index] = 0;
    pos->material[index] = 0;
    pos->pawns[index] = 0ULL;
  }

  // Reset the piece counts for all piece types
  for (index = 0; index < 13; ++index)
  {
    pos->pceNum[index] = 0;
  }

  // Reset the king square locations for both white and black
  pos->kingSq[WHITE] = pos->kingSq[BLACK] = NO_SQ;

  // Set the current side to BOTH (no side selected yet)
  pos->side = BOTH;
  // No en passant square
  pos->enPas = NO_SQ;
  // Reset the fifty-move counter
  pos->fiftyMove = 0;

  // Reset ply counters
  pos->ply = 0;
  pos->hisPly = 0;

  // Clear castle permissions
  pos->castlePerm = 0;

  // Clear the position key
  pos->posKey = 0ULL;
}

// Print the current state of the chessboard
void PrintBoard(const S_BOARD *pos)
{
  int sq, file, rank, piece;

  printf("\nGame Board:\n\n");

  // Loop through ranks 8 to 1 (top to bottom)
  for (rank = RANK_8; rank >= RANK_1; rank--)
  {
    printf("%d  ", rank + 1); // Print rank number
    // Loop through files A to H (left to right)
    for (file = FILE_A; file <= FILE_H; file++)
    {
      sq = FR2SQ(file, rank);        // Convert file and rank to square number
      piece = pos->pieces[sq];       // Get the piece at that square
      printf("%3c", PceChar[piece]); // Print the piece's character representation
    }
    printf("\n");
  }

  // Print the file labels (a-h) at the bottom of the board
  printf("\n   ");
  for (file = FILE_A; file <= FILE_H; file++)
  {
    printf("%3c", 'a' + file); // File labels from 'a' to 'h'
  }
  printf("\n");

  // Print additional game information: side to move, en passant, castling, position key
  printf("side:%c\n", SideChar[pos->side]); // Print which side is to move (White/Black)
  printf("enPas:%d\n", pos->enPas);         // Print en passant square (if any)
  printf("castle:%c%c%c%c\n",
         pos->castlePerm & WKCA ? 'K' : '-',  // White king-side castle permission
         pos->castlePerm & WQCA ? 'Q' : '-',  // White queen-side castle permission
         pos->castlePerm & BKCA ? 'k' : '-',  // Black king-side castle permission
         pos->castlePerm & BQCA ? 'q' : '-'); // Black queen-side castle permission
  printf("PosKey:%llX\n", pos->posKey);       // Print the position key in hexadecimal format
}
