#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "common.h"
#include "LLMManager.h"

namespace ai_char_sdk
{
    class ChatSDK
    {
    public:
        // 初始化SDK
        bool initialize(const APIConfig &config);
        // 检测模型是否可用
        bool isModelAvailable(const std::string &modelName);
        // 发送消息并获取回复
        std::string sendMessage(const std::string &message);
        // 获取对话历史
        std::vector<Message> getConversationHistory() const;
        // 注册消息回调函数
        void registerMessageCallback(const std::function<void(const Message &)> &callback);

    private:
        LLMManager _llmManager;                                // 和模型进行交互
        SessionManager _sessionManager;                        // 会话管理器
        std::vector<Message> _conversationHistory;             // 对话历史
        std::map<std::string, ModelInfo> _modelRegistry;       // 模型注册表
        std::function<void(const Message &)> _messageCallback; // 消息回调
    };

} // end ai_char_sdk