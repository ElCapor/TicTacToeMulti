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
    SECTION("Singleton")
    {
        auto& instance = EventManager<TestEvent>::getInstance();
        REQUIRE(instance.m_listeners.size() == 0);
    }

    SECTION("Event Listeners")
    {
        auto& instance = EventManager<TestEvent>::getInstance();
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
    }
}