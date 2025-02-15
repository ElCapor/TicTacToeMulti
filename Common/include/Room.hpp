/**
 * @file Room.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Room system for matchmaking
 * @version 0.1
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ROOM_HPP
#define ROOM_HPP
#include <vector>
#include <type_traits>
#include <Result.hpp>

// Helper trait to check if `T` has a member `id` of type `int`
template <typename T, typename = void>
struct has_int_id : std::false_type {};

template <typename T>
struct has_int_id<T, std::void_t<decltype(std::declval<T>().id)>>
    : std::is_same<decltype(std::declval<T>().id), int> {};

/**
 * @brief A room used for matchmaking
 * 
 * @tparam _Plr The type of objects you store in a room , usually a player
 */
template <class _Plr>
class Room
{
    static_assert(
        std::is_same_v<typename _Plr::id_type, int>,
        "_Plr::id_type must be exactly int"
    );
    static_assert(
        has_int_id<_Plr>::value,
        "_Plr must have a member 'id' of type int"
    );
private:
    int id;
    int maxPlayers;
    std::vector<_Plr> players;
    
public:
    Room(int _id, int _maxPlayers) : id(_id), maxPlayers(_maxPlayers) {
        players.reserve(maxPlayers);
    }
    ~Room() {}

    int GetID() const {
         return id;
    }

    int GetMaxPlayers() const {
        return maxPlayers;
    }

    int GetPlayerCount() const {
        return players.size();
    }
    
    void AddPlayer(_Plr player) {
        players.push_back(player);
    }

    void RemovePlayer(_Plr player) {
        // find player by id then remove it
        for (auto it = players.begin(); it != players.end(); ++it) {
            if (it->id == player.id) {
                players.erase(it);
                break;
            }
        }
    }

    std::vector<_Plr> GetPlayers() const {
        return players;
    }

    void ClearPlayers() {
        players.clear();
    }

    bool IsFull() const {
        return players.size() == maxPlayers;
    }

    bool IsEmpty() const {
        return players.size() == 0;
    }

    cpp::result<_Plr, int> GetPlayer(int id) const {
        for (auto it = players.begin(); it != players.end(); ++it) {
            if (it->id == id) {
                return *it;
            }
        }
        return cpp::fail(-1);
    }
    
    void SetPlayer(int id, _Plr player) {
        for (auto it = players.begin(); it != players.end(); ++it) {
            if (it->id == id) {
                *it = player;
                return;
            }
        }
    }
};

#endif // !ROOM_HPP