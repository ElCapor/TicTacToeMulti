/**
 * @file TicTac.hpp
 * @author ElCapor
 * @brief Implementation of the main tic tac toe game logic shared by server and client
 * @version 0.1
 * @date 2025-03-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <vector>
#include <math.h>

enum PawnType : int {
    PawnType_None,
    PawnType_X,
    PawnType_O,
  };
  
  class Move{
  public:
    int playerID;
    PawnType type;
    unsigned int move; // the encoded move
  };
  
 
  // A tic tac toe grid repersented in 1D
 class TicTacGrid
 {
protected:
   std::vector<Move> moves; // the moves that lead to the current grid state 
   PawnType grid[12]; // encoded moves data
public:
  TicTacGrid(int size);
  // was the move applied ?
  bool ApplyMove(Move move);
  bool IsFull();
  PawnType CheckWinner();

  // Marking as constexpr optimizes CheckWinner
  // cuz they will be evaluated at compile time
  static constexpr unsigned int encode_grid_pos(int x, int y);
  static constexpr unsigned int decode_grid_pos_x(unsigned int pos);
  static constexpr unsigned int decode_grid_pos_y(unsigned int pos);
 };
