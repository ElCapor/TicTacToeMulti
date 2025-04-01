/**
 * @file DrawableGrid.hpp
 * @author elcapor
 * @brief A drawable grid
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DRAWABLE_GRID_HPP
#define DRAWABLE_GRID_HPP

#include <TicTac.hpp>

class DrawableGrid : public TicTacGrid
{
    Vector2 pos;
    int grid_size;
    static Texture2D gridTexture;
public:
    DrawableGrid(int drawSize) : TicTacGrid(12)
    {
        grid_size = drawSize;
        for (int i = 0; i < 12; i++)
        {
            grid[i] = PawnType_None;
        }

        if (!IsTextureValid(gridTexture))
        {
            Image board = GenImageChecked(grid_size*3, grid_size*3, grid_size, grid_size, DARKGRAY, LIGHTGRAY);
            gridTexture = LoadTextureFromImage(board);
        }
    };

    void DrawO(int gridX_index, int gridY_index)
{
 DrawCircle(grid_size*gridX_index + pos.x + grid_size/2, pos.y + grid_size*gridY_index + grid_size/2, grid_size/2 - 5, RED);
}

void DrawX(int gridX_index, int gridY_index)
{
  DrawLine(
    pos.x + grid_size*gridX_index + 5,
    pos.y + grid_size*gridY_index + 5,
    pos.x + grid_size*(gridX_index + 1) - 5,
    pos.y + grid_size*(gridY_index + 1) - 5,
    RED
  );

  DrawLine(
    pos.x + grid_size*(gridX_index + 1) - 5,
    pos.y + grid_size*gridY_index + 5,
    pos.x + grid_size*gridX_index + 5,
    pos.y + grid_size*(gridY_index + 1) - 5,
    RED
  );
}

void Clear()
  {
    for (int i = 0; i < 12; i++) {
      grid[i] = PawnType_None;
    }
  }

  Vector2 GetPos()
  {
    return pos;
  }

  void SetPos(Vector2 pos)
  {
    this->pos = pos;
  }

  Rectangle GetBounds()
  {
    return Rectangle(pos.x, pos.y, grid_size*3, grid_size*3);
  }

  int GetGridSize()
  {
    return grid_size;
  }

  bool isFull()
  {
    for (int i = 0; i < 12; i++) {
      if (grid[i] == PawnType_None) {
        return false;
      }
    }
  }

  void Draw()
  {
    DrawTexture(gridTexture, pos.x, pos.y, WHITE);
    for (int i = 0; i < 12; i++) {
      switch (grid[i]) {
        case PawnType_None:
          break;
        case PawnType_X:
          DrawX(decode_grid_pos_x(i), decode_grid_pos_y(i));
          break;
        case PawnType_O:
          DrawO(decode_grid_pos_x(i), decode_grid_pos_y(i));
          break;
      }
    }
  }
};
#endif