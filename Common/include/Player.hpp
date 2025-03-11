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
    using id_type = int;

    // == operator
    bool operator==(const Player& other) const
    {
        return id == other.id;
    }

    bool operator!=(const Player& other) const
    {
        return !(*this == other);
    }
};


#endif // PLAYER_HPP