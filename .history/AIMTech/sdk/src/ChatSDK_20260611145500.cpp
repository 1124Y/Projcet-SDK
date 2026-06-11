#include "../include/ChatSDK.h"
#include "../include/util/myLog.h"
#include "../include/DeepSeekProvider.h"
#include "../include/OllamaLLMProvider.h"
namespace ai_chat_sdk
{
    // 初始化模型
    bool ChatSDK::initModels(const std::vector<std::shared_ptr<Config>> &configs)
    {
        // 注册所支持的模型
        registerAllProvider(configs);

        // 初始化所支持的模型
        initProviders(configs);

        _initialized = true;
        return true;
    }

    // 注册所支持的模型
    void ChatSDK::registerAllProvider(const std::vector<std::shared_ptr<Config>> &configs)
    {
        // deepseek-chat模型提供者注册
        if (!_llmManager.isModelAvailable("deepseek-chat"))
        {
            auto deepseekProvider = std::make_unique<DeepSeekProvider>();

            // unuqe_ptr不允许自动类型转换，即使类型之间存在继承关系
            _llmManager.registerProvider("deepseek-chat", std::move(deepseekProvider));
            INFO("deepseek-chat provider registered successed");
        }
        // Ollama接入本地模型---模型信息通过用户传递 configs
        for ()
    }
    // 初始化所支持的模型
    void ChatSDK::initProviders(const std::vector<std::shared_ptr<Config>> &configs)
    {
    }

    // 创建会话
    std::string ChatSDK::createSession(const std::string &modelName)
    {
    }

    // 获取指定会话
    std::shared_ptr<Session> ChatSDK::getSession(const std::string &sessionId)
    {
    }

    // 获取所有会话列表
    std::vector<std::string> ChatSDK::getSessionLists() const
    {
    }

    // 删除指定会话
    bool ChatSDK::deleteSession(const std::string &sessionId)
    {
    }
} // end ai_chat_sdk