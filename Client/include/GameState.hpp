/**
 * @file GameState.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Game state enum
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState
{
	WaitingForServer,
    WaitingForPlayers,
    GamePlayerReady,
    GameStart,
    PlayerTurn,
    EnemyTurn,
    GameWin,
    GameLose,
    GameRematch
};

#endif // GAMESTATE_H