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
        std::string _modelName;    // 模型名称
        double _temperature = 0.7; // 温度参数
        int _maxTokens = 2048;     // 最大生成长度

        float _topP; // nucleus采样的概率阈值
        int _n;      // 生成的候选数量

        // 构造函数
        Config(const std::string &modelName, double temperature, int maxTokens, float topP, int n)
            : _modelName(modelName),
              _maxTokens(maxTokens),
              _temperature(temperature),
              _topP(topP),
              _n(n)
        {
        }
    }

    // 通过API方式调用模型的输入参数

    // 通过Ollama方式调用模型的输入参数

}
