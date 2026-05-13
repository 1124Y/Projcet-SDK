#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

namespace ai_chat_sdk
{
    // 消息结构
    struct Message
    {
        std::string _messageId; // 消息ID
        std::string _role;      // 角色：system、user、assistant
        std::string _content;   // 消息内容
        std::time_t _timestamp; // 时间戳
    };
}
