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
        Message(const std::string &role = "", const std::string &content = "")
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
    };

    // 通过API方式调用模型的输入参数
    struct APIConfig : public Config
    {
        std::string _apiKey; // API密钥
        // std::string _endpoint; // API端点URL
    };

    // 通过Ollama方式调用模型的输入参数
    struct OllamaConfig : public Config
    {
        std::string _modelName; // 本地模型名称
        std::string _modelDesc; // 模型描述信息
        std::string _endpoint;  // Ollama API端点URL
    };

    // LLM信息
    struct ModelInfo
    {
        std::string _modelName;    // 模型名称
        std::string _modelDesc;    // 模型描述
        std::string _provider;     // 模型提供者
        std::string _endpoint;     // 模型API端点URL
        bool _isAvailable = false; // 模型是否可用

        ModelInfo(const std::string &modelName = "", const std::string &modelDesc = "", const std::string &provider = "", const std::string &endpoint = "")
            : _modelName(modelName),
              _modelDesc(modelDesc),
              _provider(provider),
              _endpoint(endpoint)
        {
        }
    };

    // 会话
    struct Session
    {
        std::string _sessionId;         // 会话ID
        std::string _modelName;         // 模型名称
        std::vector<Message> _messages; // 消息列表

        std::time_t _createdAt; // 会话创建时间
        std::time_t _updatedAt; // 会话更新时间

        Session(const std::string &modelName)
            : _modelName(modelName)
        {
        }
    };

}
