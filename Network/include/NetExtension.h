/**
 * @file NetExtension.h
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Useful extensions to the network module
 * @version 0.1
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef NET_EXTENSION_H
#define NET_EXTENSION_H

#include <NetBase.h>

namespace net
{
    template <typename T>
    message<T> new_message(T message_type)
    {
        message<T> msg;
        msg.header.id = message_type;
        return msg;
    }
}


#endif // NET_EXTENSION_H