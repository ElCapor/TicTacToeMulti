/**
 * @file Player.hpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief Player struct
 * @version 0.1
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef PLAYER_HPP
#define PLAYER_HPP

struct Player
{
    int id;
    using id_type = typename int;
    Player(int id) : id(id) {}
};


#endif // PLAYER_HPP