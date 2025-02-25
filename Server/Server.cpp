/**
 * @file Server.cpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief Server Entry Point
 * @version 0.1
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <NetServer.hpp>

int main()
{
    NetServer server(62323);
    server.Start();
    while (true)
    {
        server.Update();
    }
    return 0;
}