/**
 * @file Singleton.hpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief  Singleton Header
 * @version 0.1
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef SINGLETON_HPP
#define SINGLETON_HPP
/*
SINGLETON Implementation
*/
template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance; // This will be created only once
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent duplication
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};
#endif