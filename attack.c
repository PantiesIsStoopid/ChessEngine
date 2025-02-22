// attack.c

#include "stdio.h"
#include "defs.h"

// Direction arrays for various pieces' movements
const int KnDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };  // Knight directions
const int RkDir[4] = { -1, -10, 1, 10 };                      // Rook directions
const int BiDir[4] = { -9, -11, 11, 9 };                      // Bishop directions
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };      // King directions

// Function to check if a square is attacked
int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

    int pce, index, t_sq, dir;

    // Check if the square is attacked by a pawn
    if (side == WHITE) {
        if (pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
            return TRUE;  // Square is attacked by white pawn
        }
    } else {
        if (pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP) {
            return TRUE;  // Square is attacked by black pawn
        }
    }

    // Check if the square is attacked by a knight
    for (index = 0; index < 8; ++index) {        
        pce = pos->pieces[sq + KnDir[index]];
        if (IsKn(pce) && PieceCol[pce] == side) {
            return TRUE;  // Square is attacked by a knight
        }
    }

    // Check if the square is attacked by a rook or queen
    for (index = 0; index < 4; ++index) {        
        dir = RkDir[index];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if (pce != EMPTY) {
                if (IsRQ(pce) && PieceCol[pce] == side) {
                    return TRUE;  // Square is attacked by a rook or queen
                }
                break;  // Blocked by another piece
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // Check if the square is attacked by a bishop or queen
    for (index = 0; index < 4; ++index) {        
        dir = BiDir[index];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if (pce != EMPTY) {
                if (IsBQ(pce) && PieceCol[pce] == side) {
                    return TRUE;  // Square is attacked by a bishop or queen
                }
                break;  // Blocked by another piece
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // Check if the square is attacked by a king
    for (index = 0; index < 8; ++index) {        
        pce = pos->pieces[sq + KiDir[index]];
        if (pce != OFFBOARD && IsKi(pce) && PieceCol[pce] == side) {
            return TRUE;  // Square is attacked by a king
        }
    }

    return FALSE;  // Square is not attacked
}
