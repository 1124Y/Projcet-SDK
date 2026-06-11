#include "../include/ChatSDK.h"
#include "../include/util/myLog.h"
#include "../include/DeepSeekProvider.h"
#include "../include/OllamaLLMProvider.h"
#include <memory>
#include <unordered_set>

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
        std::unordered_set<std::string> modelNames; // 用于记录已经注册的模型名称，避免重复注册
        for (const auto &config : configs)
        {
            auto ollamaConfig = std::dynamic_pointer_cast<OllamaConfig>(config); // 向下转型，判断用户传递的配置是否为OllamaConfig类型，如果是则注册Ollama模型提供者
            if (ollamaConfig)
            {
                auto modelName = ollamaConfig->_modelName;
                if (modelNames.find(modelName) == modelNames.end())
                {
                    // 该模型还没有注册
                    modelNames.insert(modelName);

                    if (!_llmManager.isModelAvailable(modelName))
                    {
                        // 该模型还没有注册
                        _llmManager.registerProvider(modelName, std::make_unique<OllamaLLMProvider>());
                        INFO("OllamaLLMProvider {} registered successed", modelName);
                    }
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