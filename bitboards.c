// bitboards.c

#include "stdio.h"
#include "defs.h"

// Predefined bit table for popcount optimization
const int BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33,
    15, 50, 42, 13, 11, 53, 19, 34,
    61, 29, 2, 51, 21, 43, 45, 10,
    18, 47, 1, 54, 9, 57, 0, 35,
    62, 31, 40, 4, 49, 5, 52, 26,
    60, 6, 23, 44, 46, 27, 56, 16,
    7, 39, 48, 24, 59, 14, 12, 55,
    38, 28, 58, 20, 37, 17, 36, 8};

// Function to pop the least significant bit and update the bitboard
int PopBit(U64 *bb) {
    U64 b = *bb ^ (*bb - 1);  // Isolate the least significant bit
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));  // Fold to calculate the position
    *bb &= (*bb - 1);  // Remove the least significant bit
    return BitTable[(fold * 0x783a9b23) >> 26];  // Return the index from the bit table
}

// Function to count the number of bits set to 1 in the bitboard
int CountBits(U64 b) {
    int r;
    for (r = 0; b; r++, b &= b - 1)  // Iterate and clear the least significant bit
        ;
    return r;  // Return the bit count
}

// Function to print the bitboard in a human-readable format
void PrintBitBoard(U64 bb) {
    U64 shiftMe = 1ULL;  // Mask for shifting the bitboard
    int rank = 0;
    int file = 0;
    int sq = 0;
    int sq64 = 0;

    printf("\n");
    // Loop through ranks and files (from top to bottom, left to right)
    for (rank = RANK_8; rank >= RANK_1; --rank) {
        for (file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file, rank);  // Convert file and rank to 120-based square
            sq64 = SQ64(sq);         // Convert to 64-based square
            // Check if the bit is set at the given square
            if ((shiftMe << sq64) & bb)
                printf("X");  // Print "X" if the square is occupied
            else
                printf("-");  // Print "-" if the square is empty
        }
        printf("\n");
    }
    printf("\n\n");
}
