#pragma once
#include "common.h"
#include "LLMManager.h"
#include "SessionManager.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace ai_char_sdk
{
    class ChatSDK
    {
    public:
        // 初始化SDK
        virtual bool initSDK(const std::map<std::string, std::string> &sdkConfig) = 0;
        // 检测模型是否可用
        bool isModelAvailable(const std::string &modelName);

    private:
        LLMManager _llmManager;                                // 和模型进行交互
        SessionManager _sessionManager;                        // 和会话进行交互
        std::vector<Message> _conversationHistory;             // 对话历史
        std::map<std::string, ModelInfo> _modelRegistry;       // 模型注册表
        std::function<void(const Message &)> _messageCallback; // 消息回调
    };

} // end ai_char_sdk