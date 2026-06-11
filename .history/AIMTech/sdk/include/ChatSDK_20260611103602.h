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
        bool _initialized = false;      // SDK是否已初始化
        LLMManager _llmManager;         // 和模型进行交互
        SessionManager _sessionManager; // 和会话进行交互
    };

} // end ai_char_sdk