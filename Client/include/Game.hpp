/**
 * @file Game.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Base Game Class
 * @version 0.1
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef GAME_H
#define GAME_H
#include <memory>
#include <GameState.hpp>

class Game
{
int m_Width;
int m_Height;
const char* m_windowTitle;

GameState m_GameState;
public:
    Game(int width, int height, const char* windowTitle);
    void OnLoad();
    void OnExit();
    void OnUpdate();
    void OnDraw();
    void Main();
};

#endif // GAME_H