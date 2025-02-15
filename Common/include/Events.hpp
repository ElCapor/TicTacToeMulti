/**
 * @file Events.hpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief Header-Only Events System
 * @version 0.1
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <unordered_set>
#include <vector>
#include <map>
/*
https://www.linkedin.com/pulse/students-take-implementing-event-systems-games-using-matthew-rosen

Took the headers and implenting all the functions by myself - elcapor
*/

template<typename T>
using UniqueContainer = std::unordered_set<T>;

template <typename EventType>
class Event
{
public:
  explicit Event(EventType type) : m_type(type) {}
  EventType m_type;
};

template <typename EventType>
class EventListener
{
public:
  virtual void OnEvent(Event<EventType> *received) = 0;
  void Subscribe(EventType type)
  {
    if (this->m_subscribeList.find(type) == this->m_subscribeList.end())
    {
        this->m_subscribeList.emplace(type);
    }
  }
  void Unsubscribe(EventType type)
  {
    for (auto it = m_subscribeList.begin(); it!=m_subscribeList.end();)
    {
        if (*it == type)
        {
            it = m_subscribeList.erase(it);
        }
        else {
            it++;
        }
    }
  }
  void UnsubscribeAll()
  {
    this->m_subscribeList.clear();
  }

  virtual ~EventListener() { UnsubscribeAll();}


private:
  UniqueContainer<EventType> m_subscribeList;
};

template <typename EventType>
class EventManager
{
public:
  void Init();
  void Update();
  void Exit();

  void RemoveListener(EventListener<EventType> *listener)
  {
    // this is a sort of unsubscribe but we don't know what event type the listener is bound to, so we just
    // remove the listener from all events if there's any
    for (auto& event : m_listeners)
    {
        event.second.erase(std::remove(event.second.begin(), event.second.end(), listener), event.second.end());
    }
  }
  void SendEvent(Event<EventType> *sent)
  {
    for (auto listener : m_listeners[sent->m_type])
    {
        listener->OnEvent(sent);
    }
  }

  // future functionality:// void QueueEvent(Event *sent, float delay = 0.0f);  
  friend class EventListener<EventType>;
  void Subscribe(EventType type, EventListener<EventType> *listener)
  {
    m_listeners[type].emplace_back(listener);
  }
  void Unsubscribe(EventType type, EventListener<EventType> *listener)
  {
    for (auto it = m_listeners[type].begin(); it != m_listeners[type].end();)
    {
        if (*it == listener)
        {
            it = m_listeners[type].erase(it);
        }
        else {
            it++;
        }
    }
  }

  std::map<EventType, std::vector<EventListener<EventType>*>> m_listeners;
};

#endif