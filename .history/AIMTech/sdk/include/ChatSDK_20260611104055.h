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
        // 初始化支持的模型
        bool ChatSDK::initModels(const std::vector<std::shared_ptr<Config>> &configs)
        {
            // 注册所支持的模型
            registerAllProvider(configs);
            // 初始化所支持的模型
            initProviders(configs);

            _initialized = true;
            return true;
        }

    private:
        bool _initialized = false; // SDK是否已初始化

        LLMManager _llmManager;         // 和模型进行交互
        SessionManager _sessionManager; // 和会话进行交互
    };

} // end ai_char_sdk