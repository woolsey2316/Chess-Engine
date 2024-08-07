#include <stdio.h>

typedef unsigned long long U64;

enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1
};

enum { white, black };
// sides to move (colours)
// not A file constant

// set/get/pop macros
#define set_bit(bitboard, square) bitboard |= (1ULL << square)
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0);

static inline int count_bits(U64 bitboard)
{
  int count = 0;

  while (bitboard)
  {
    count++;
    bitboard &= bitboard - 1;
  }

  return count;
}

static inline int get_ls1b_index(U64 bitboard)
{
  if (bitboard)
  {
    return count_bits((bitboard & -bitboard) - 1);
  }
  else {
    return -1;
  }
}
// print bitboard
void print_bitboard(U64 bitboard)
{
	//loop over board ranks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			// convert file & rank into square index
			int square = rank * 8 + file;
			
			// print ranks
			if (!file)
				printf(" %d ", 8 - rank);

			// print bit state (either 1 or 0)
			printf(" %d" , get_bit(bitboard, square) ? 1 : 0);
		}
		printf("\n");
	}
	printf("\n    a b c d e f g h\n\n");
	// print bitboard as unsigned decimal number
	printf("     Bitboard: %llud\n\n",bitboard);
}
// not A file constant
/* 
 8  0 1 1 1 1 1 1 1
 7  0 1 1 1 1 1 1 1
 6  0 1 1 1 1 1 1 1
 5  0 1 1 1 1 1 1 1
 4  0 1 1 1 1 1 1 1
 3  0 1 1 1 1 1 1 1
 2  0 1 1 1 1 1 1 1
 1  0 1 1 1 1 1 1 1

    a b c d e f g h
*/
/*
 8  1 1 1 1 1 1 1 0
 7  1 1 1 1 1 1 1 0
 6  1 1 1 1 1 1 1 0
 5  1 1 1 1 1 1 1 0
 4  1 1 1 1 1 1 1 0
 3  1 1 1 1 1 1 1 0
 2  1 1 1 1 1 1 1 0
 1  1 1 1 1 1 1 1 0

    a b c d e f g h
*/
/* 
 8  1 1 1 1 1 1 0 0
 7  1 1 1 1 1 1 0 0
 6  1 1 1 1 1 1 0 0
 5  1 1 1 1 1 1 0 0
 4  1 1 1 1 1 1 0 0
 3  1 1 1 1 1 1 0 0
 2  1 1 1 1 1 1 0 0
 1  1 1 1 1 1 1 0 0

    a b c d e f g h
*/
/*
 8  0 0 1 1 1 1 1 1
 7  0 0 1 1 1 1 1 1
 6  0 0 1 1 1 1 1 1
 5  0 0 1 1 1 1 1 1
 4  0 0 1 1 1 1 1 1
 3  0 0 1 1 1 1 1 1
 2  0 0 1 1 1 1 1 1
 1  0 0 1 1 1 1 1 1

    a b c d e f g h
*/
// not H File constant
const U64 not_a_file = 18374403900871474942ULL;

const U64 not_h_file = 9187201950435737471ULL;

const U64 not_hg_file = 4557430888798830399LL;

const U64 not_ab_file = 18229723555195321596ULL;

const int bishop_relevant_bits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6,
};

const int rook_relevant_bits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12,
};
// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

U64 knight_attacks[64];

U64 King_attacks[64];

const char *square_to_coordinates[] = {
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"

};

U64 mask_pawn_attacks(int side, int square)
{
  // result attacks bitboard
  U64 attacks = 0ULL;
  
  // piece bitboard
  U64 bitboard = 0ULL;
 
  // set piece on board
  set_bit(bitboard, square);

  // white pawn_attacks
  if (!side) {
    // generate pawn attaks
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
 
  }

  // black pawns
  else {
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);

  }
  // return attack mask_pawn_attacks
  return attacks;
}
// generate knight attacks
U64 mask_knight_attacks(int square)
{
  U64 attacks = 0ULL;

  U64 bitboard = 0ULL;

  set_bit(bitboard, square);

  if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
  if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
  if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
  if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

  if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
  if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
  if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
  if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

  return attacks;
}

U64 mask_king_attacks(int square)
{
  U64 attacks = 0ULL;

  U64 bitboard = 0ULL;

  set_bit(bitboard, square);

  if ((bitboard >> 8)) attacks |= (bitboard >> 8);
  if ((bitboard >> 7) & not_h_file) attacks |= (bitboard >> 7);
  if ((bitboard >> 9) & not_a_file) attacks |= (bitboard >> 9);
  if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);

  if ((bitboard << 8)) attacks |= (bitboard << 8);
  if ((bitboard << 7) & not_a_file) attacks |= (bitboard << 7);
  if ((bitboard << 9) & not_h_file) attacks |= (bitboard << 9);
  if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

  return attacks;

}

U64 mask_bishop_attacks(int square)
{
  // result attacks bitboard
  U64 attacks = 0ULL;
  
  int r, f;

  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) {
    attacks |= (1ULL << (r * 8 + f));
  }
  for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++){
    attacks |= (1ULL << (r * 8 + f));
  }
  for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--){
    attacks |= (1ULL << (r * 8 + f));
  }
  for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--){
    attacks |= (1ULL << (r * 8 + f));
  }
  
  return attacks;

}

U64 mask_rook_attacks(int square)
{

  U64 attacks = 0ULL;
  
  int r, f;

  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
  for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
  for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
  for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));
 
  return attacks;

}

U64 bishop_attacks_on_the_fly(int square, U64 block)
{
  // result attacks bitboard
  U64 attacks = 0ULL;
  
  int r, f;

  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
    attacks |= (1ULL << (r * 8 + f));
    if (1ULL << (r * 8 + f) & block) break;
  }
  for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++){
    attacks |= (1ULL << (r * 8 + f));
    if (1ULL << (r * 8 + f) & block) break;
  }
  for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--){
    attacks |= (1ULL << (r * 8 + f));
    if (1ULL << (r * 8 + f) & block) break;
  }
    attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--){
    attacks |= (1ULL << (r * 8 + f));
    if (1ULL << (r * 8 + f) & block) break;
  }
  
  return attacks;

}

U64 rook_attacks_on_the_fly(int square, U64 block)
{

  U64 attacks = 0ULL;
  
  int r, f;

  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1; r <= 7; r++) {
    attacks |= (1ULL << (r * 8 + tf));
    if (1ULL << (r * 8 + tf) & block) break;
  }
  for (r = tr - 1; r >= 0; r--) {
    attacks |= (1ULL << (r * 8 + tf));
    if (1ULL << (r * 8 + tf) & block) break;
  }
  for (f = tf + 1; f <= 7; f++) {
    attacks |= (1ULL << (tr * 8 + f));
    if (1ULL << (r * 8 + tf) & block) break;
  }
  for (f = tf - 1; f >= 0; f--) {
    attacks |= (1ULL << (tr * 8 + f));
    if (1ULL << (r * 8 + tf) & block) break;
  }
 
  return attacks;

}

void init_leapers_attacks()
{
  // loop over 64 board squares
  for (int square = 0; square < 64; square++) {
    pawn_attacks[white][square] = mask_pawn_attacks(white, square);
    pawn_attacks[black][square] = mask_pawn_attacks(black, square);

    knight_attacks[square] = mask_knight_attacks(square);
  }

}

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{
  U64 occupancy = 0ULL;

  for (int count = 0; count < bits_in_mask; count++)
  {
    // get LS1B index of attacks mask_bishop_attacks
    int square = get_ls1b_index(attack_mask);

    pop_bit(attack_mask, square);

    if (index & (1 << count))
    {
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}

unsigned int state = 1804289383;

unsigned int get_random_number() 
{
  
  unsigned int number = state;

  number ^= number << 13;
  number ^= number >> 17;
  number ^= number << 5;

  state = number;

  return number;
}


int main()
{
  init_leapers_attacks();

  printf("%ud\n", get_random_number());
  printf("%ud\n", get_random_number());
  printf("%ud\n", get_random_number());
  printf("%ud\n", get_random_number());
  return 0;
}
