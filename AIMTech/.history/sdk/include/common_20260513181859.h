#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace ai_chat_sdk
{
    // 消息结构
    struct Message
    {
        std::string role;    // 角色：system、user、assistant
        std::string content; // 消息内容
    };
}
