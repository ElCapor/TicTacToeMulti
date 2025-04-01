/**
 * @file RoomGame.hpp
 * @author ElCapor (github.com/ElCapor)
 * @brief 
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Room.hpp"
#include "TicTac.hpp"

template <typename _Plr>
class RoomGame : public Room<_Plr> {
public:
    TicTacGrid grid;

};