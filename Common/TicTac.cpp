#include "TicTac.hpp"

TicTacGrid::TicTacGrid(int size)
{
    for (int i = 0; i < size; i++)
        grid[i] = PawnType_None;
}

bool TicTacGrid::ApplyMove(Move move)
{
    if (move.move < 0 || move.move >= 12)
    {
        return false;
    }

    int pos = move.move;
    PawnType type = move.type;
    if (grid[pos] != PawnType_None)
    {
        return false;
    }
    grid[pos] = type;
    moves.push_back(move);
    return true;
}

bool TicTacGrid::IsFull()
{
    for (int i = 0; i < 12; i++)
        if (grid[i] == PawnType_None)
            return false;
    return true;
}

PawnType TicTacGrid::CheckWinner()
{
    // Check rows
    for (int y = 0; y < 3; ++y) {
        if (grid[encode_grid_pos(0, y)] != PawnType_None &&
            grid[encode_grid_pos(0, y)] == grid[encode_grid_pos(1, y)] &&
            grid[encode_grid_pos(1, y)] == grid[encode_grid_pos(2, y)]) {
            return grid[encode_grid_pos(0, y)];
        }
    }

    // Check columns
    for (int x = 0; x < 3; ++x) {
        if (grid[encode_grid_pos(x, 0)] != PawnType_None &&
            grid[encode_grid_pos(x, 0)] == grid[encode_grid_pos(x, 1)] &&
            grid[encode_grid_pos(x, 1)] == grid[encode_grid_pos(x, 2)]) {
            return grid[encode_grid_pos(x, 0)];
        }
    }

    // Check diagonals
    if (grid[encode_grid_pos(0, 0)] != PawnType_None &&
        grid[encode_grid_pos(0, 0)] == grid[encode_grid_pos(1, 1)] &&
        grid[encode_grid_pos(1, 1)] == grid[encode_grid_pos(2, 2)]) {
        return grid[encode_grid_pos(0, 0)];
    }
    if (grid[encode_grid_pos(0, 2)] != PawnType_None &&
        grid[encode_grid_pos(0, 2)] == grid[encode_grid_pos(1, 1)] &&
        grid[encode_grid_pos(1, 1)] == grid[encode_grid_pos(2, 0)]) {
        return grid[encode_grid_pos(0, 2)];
    }

    // No winner
    return PawnType_None;
}