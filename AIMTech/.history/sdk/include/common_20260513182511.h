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

        // 构造函数
        Message(const std::string &role, const std::string &content)
            : _role(role),
              _content(content)
        {
        }
    };

    // 模型公共配置信息
    struct Config
    {
    }

    // 通过API方式调用模型的输入参数

}
