/**
 * @file Unit.cpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief Unit Test Entry Point
 * @version 0.1
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/// Catch2 Unit Test Framework
#include <catch2/catch_test_macros.hpp>

/// Modules to test
#include <Singleton.hpp>
#include <Events.hpp>

/// Test Events
TEST_CASE("EVENTS SYSTEM", "[Events]")
{
    enum TestEvent{
        TEST_EVENT_1,
        TEST_EVENT_2,
        TEST_EVENT_3
    };

    class TestEvent1: public Event<TestEvent>
    {
    public:
        TestEvent1() : Event(TEST_EVENT_1) {}
    };

    class TestEvent2: public Event<TestEvent>
    {
    public:
        TestEvent2() : Event(TEST_EVENT_2) {}
    };

    class TestEvent3: public Event<TestEvent>
    {
    public:
        TestEvent3() : Event(TEST_EVENT_3) {}
    };


    class TestListener: public EventListener<TestEvent>
    {
    public:
        bool event1Received = false;
        bool event2Received = false;
        bool event3Received = false;
    public:
        void OnEvent(Event<TestEvent> *received) override
        {
            switch (received->m_type)
            {
                case TEST_EVENT_1:
                    event1Received = true;
                    break;
                case TEST_EVENT_2:
                    event2Received = true;
                    break;
                case TEST_EVENT_3:
                    event3Received = true;
                    break;
            }
        }
    };

    class TestEventManager: public EventManager<TestEvent>, public Singleton<TestEventManager>
    {
    public:
        TestEventManager() : EventManager<TestEvent>() {}
    };
    
    SECTION("Singleton")
    {
        auto& instance = TestEventManager::getInstance();
        REQUIRE(instance.m_listeners.size() == 0);
    }

    SECTION("Event Listeners")
    {
        auto& instance = TestEventManager::getInstance();
        TestListener listener;
        SECTION("Subscribing")
        {
            instance.Subscribe(TEST_EVENT_1, &listener);
            instance.Subscribe(TEST_EVENT_2, &listener);
            instance.Subscribe(TEST_EVENT_3, &listener);
            REQUIRE(instance.m_listeners.size() == 3);
            REQUIRE(instance.m_listeners[TEST_EVENT_1].size() == 1);
            REQUIRE(instance.m_listeners[TEST_EVENT_2].size() == 1);
            REQUIRE(instance.m_listeners[TEST_EVENT_3].size() == 1);
        }
        SECTION("Triggering")
        {
            TestEvent1 event1;
            TestEvent2 event2;
            TestEvent3 event3;
            instance.SendEvent(&event1);
            REQUIRE(listener.event1Received == true);
            instance.SendEvent(&event2);
            REQUIRE(listener.event2Received == true);
            instance.SendEvent(&event3);
            REQUIRE(listener.event3Received == true);
        }

        SECTION("Unsubscribing")
        {
            instance.Unsubscribe(TEST_EVENT_1, &listener);
            instance.Unsubscribe(TEST_EVENT_2, &listener);
            instance.Unsubscribe(TEST_EVENT_3, &listener);
            REQUIRE(instance.m_listeners[TEST_EVENT_1].size() == 0);
            REQUIRE(instance.m_listeners[TEST_EVENT_2].size() == 0);
            REQUIRE(instance.m_listeners[TEST_EVENT_3].size() == 0);
        }

        SECTION("Removing Listener")
        {
            // Nothing bad should happen if we remove a listener that isn't subscribed to anything
            REQUIRE_NOTHROW(instance.RemoveListener(&listener));

            instance.Subscribe(TEST_EVENT_1, &listener);
            REQUIRE(instance.m_listeners[TEST_EVENT_1].size() == 1);
            instance.RemoveListener(&listener);
            REQUIRE(instance.m_listeners[TEST_EVENT_1].size() == 0);
        }
    }
}

/// Test Room
#include <Room.hpp>

TEST_CASE("ROOM SYSTEM", "[Room]")
{
    struct Player{
        int id;
        using id_type = typename int;
    };

    Room<Player> room(0,2);
    REQUIRE(room.GetID() == 0);
    REQUIRE(room.GetMaxPlayers() == 2);
    REQUIRE(room.GetPlayerCount() == 0);

    Player player1{1};
    Player player2{2};
    room.AddPlayer(player1);
    room.AddPlayer(player2);
    REQUIRE(room.GetPlayerCount() == 2);
    REQUIRE(room.IsFull() == true);

    room.RemovePlayer(player1);
    REQUIRE(room.GetPlayerCount() == 1);

    room.RemovePlayer(player2);
    REQUIRE(room.GetPlayerCount() == 0);

    room.AddPlayer(player1);
    REQUIRE(room.GetPlayerCount() == 1);

    auto player = room.GetPlayer(1);
    REQUIRE(player.has_value());
    REQUIRE(player.value().id == 1);
    REQUIRE(room.IsFull() == false);
    REQUIRE(room.IsEmpty() == false);

    room.ClearPlayers();
    REQUIRE(room.IsFull() == false);
    REQUIRE(room.IsEmpty() == true);
}

#include <RoomManager.hpp>
TEST_CASE("ROOM MANAGER SYSTEM", "[RoomManager]")
{
    RoomManager roomManager;
    REQUIRE(roomManager.GetAvailableRoomIndex() == 0);

    Player player1{1};
    Player player2{2};
    Player player3{3};
    Player player4{4};

    SECTION("Adding & Removing Players")
    {
        roomManager.AssignNewRoomToPlayer(player1);
        REQUIRE(roomManager.GetAvailableRoomIndex() == 0);
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().GetID() == 0));
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().IsFull() == false));
        roomManager.AssignNewRoomToPlayer(player2);
        REQUIRE(roomManager.GetAvailableRoomIndex() == -1);
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().GetID() == 0));
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().IsFull() == true));
        roomManager.AssignNewRoomToPlayer(player3);
        REQUIRE(roomManager.GetAvailableRoomIndex() == 1);
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().GetID() == 1));
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().IsFull() == false));
        roomManager.AssignNewRoomToPlayer(player4);
        REQUIRE(roomManager.GetAvailableRoomIndex() == -1);
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().GetID() == 1));
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().IsFull() == true));

        roomManager.GetRoom(0).value().SetState(RoomState_Locked);

        roomManager.RemovePlayerFromRoom(player1);
        REQUIRE(roomManager.GetAvailableRoomIndex() == -1);
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().GetID() == 0));
        REQUIRE((roomManager.GetRoom(0).has_value() && roomManager.GetRoom(0).value().IsFull() == false));
        roomManager.RemovePlayerFromRoom(player2);
        REQUIRE(roomManager.GetAvailableRoomIndex() == -1);
        REQUIRE((roomManager.GetRoom(0).has_error() == true));

        roomManager.RemovePlayerFromRoom(player3);
        REQUIRE(roomManager.GetAvailableRoomIndex() == 0);
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().GetID() == 1));
        REQUIRE((roomManager.GetRoom(1).has_value() && roomManager.GetRoom(1).value().IsFull() == false));
        roomManager.RemovePlayerFromRoom(player4);
        REQUIRE(roomManager.GetAvailableRoomIndex() == -1);
    }
    
}