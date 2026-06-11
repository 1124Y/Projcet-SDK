#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "LLMManager.h"
#include "SessionManager.h"
#include "common.h"

namespace ai_chat_sdk
{
    class ChatSDK
    {
    public:
        // 初始化模型
        bool initModels(const std::vector<std::shared_ptr<Config>> &configs);
        // 创建会话
        std::string createSession(const std::string &modelName);
        // 获取指定会话
        std::shared_ptr<Session> getSession(const std::string &sessionId);
        // 获取所有会话列表
        std::vector<std::string> getSessionLists() const;
        // 删除指定会话
        bool deleteSession(const std::string &sessionId);
        // 获取可用的模型信息
        std::vector<ModelInfo> getAvailableModels() const;

    private:
        bool _initialized = false; // SDK是否已初始化
        // key: 模型名称  value：模型配置信息
        std::unordered_map<std::string, std::shared_ptr<Config>> _modelConfigs; // 模型配置
        LLMManager _llmManager;                                                 // 主要和模型进行交互
    public:
        SessionManager _sessionManager; // 主要和会话进行交互
    };

} // end ai_chat_sdk