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
            }
        }
    }
    // 初始化所支持的模型
    void ChatSDK::initProviders(const std::vector<std::shared_ptr<Config>> &configs)
    {
        for (const auto &config : configs)
        {
            if (auto apiConfig = std::dynamic_pointer_cast<APIConfig>(config))
            {
                if (apiConfig->_modelName == "deepseek-chat")
                {
                    // 支持的云端模型
                    initAPIModelProviders(apiConfig->_modelName, apiConfig);
                }
                else
                {
                    ERR("Model {} is not supported", apiConfig->_modelName);
                }
            }
            else if (auto ollamaConfig = std::dynamic_pointer_cast<OllamaConfig>(config))
            {
                // 初始化Ollama模型提供者
                initOllamaModelProviders(ollamaConfig->_modelName, ollamaConfig);
            }
            else
            {
                ERR("Config {} is not supported", config->_modelName);
            }
        }
    }

    // 初始化模型提供者 - API模型提供者
    bool ChatSDK::initAPIModelProviders(const std::string &modelName, const std::shared_ptr<APIConfig> &apiConfig)
    {
        // 参数检测
        if (modelName.empty())
        {
            ERR("ChatSDK::initAPIModelProviders: modelName is empty");
            return false;
        }

        if (!apiConfig || apiConfig->_apiKey.empty())
        {
            ERR("ChatSDK::initAPIModelProviders: apiKey is empty");
            return false;
        }

        // 初始化模型提供者
        if (_llmManager.isModelAvailable(modelName))
        {
            INFO("ChatSDK::initAPIModelProviders: model {} is already available", modelName);
            return true;
        }

        // 初始化模型
        std::map<std::string, std::string> modelParams;
        modelParams["api_key"] = apiConfig->_apiKey;
        if (!_llmManager.initModel(modelName, modelParams))
        {
            ERR("ChatSDK::initAPIModelProviders: init model {} failed", modelName);
            return false;
        }

        // 模型配置
        _modelConfigs[modelName] = apiConfig;
        INFO("ChatSDK::initAPIModelProviders: model {} init successed", modelName);
        return true;
    }

    // 初始化模型提供者 - Ollama模型提供者
    bool ChatSDK::initOllamaModelProviders(const std::string &modelName, const std::shared_ptr<OllamaConfig> &ollamaConfig)
    {
        // 参数检测
        if (modelName.empty())
        {
            ERR("ChatSDK::initOllamaModelProviders: modelName is empty");
            return false;
        }
        if (!ollamaConfig || ollamaConfig->_endpoint.empty())
        {
            ERR("ChatSDK::initOllamaModelProviders: endpoint is empty");
            return false;
        }

        // 初始化模型提供者
        if (_llmManager.isModelAvailable(modelName))
        {
            INFO("ChatSDK::initOllamaModelProviders: model {} is already available", modelName);
            return true;
        }

        // 初始化模型
        std::map<std::string, std::string> modelParams;
        modelParams["model_name"] = modelName;
        modelParams["model_desc"] = ollamaConfig->_modelDesc;
        modelParams["endpoint"] = ollamaConfig->_endpoint;
        if (!_llmManager.initModel(modelName, modelParams))
        {
            ERR("ChatSDK::initOllamaModelProviders: init model {} failed", modelName);
            return false;
        }

        // 模型配置
        _modelConfigs[modelName] = ollamaConfig;
        INFO("ChatSDK::initOllamaModelProviders: model {} init successed", modelName);
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////

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

    // 获取可用的模型信息
    std::vector<ModelInfo> getAvailableModels()
    {
    }

    // 发送消息 - 全量返回
    std::string sendMessage(const std::string &sessionId, const std::string &message)
    {
    }
    // 发送消息 - 增量返回 - 流式响应
    std::string sendMessageStream(const std::string &sessionId, const std::string &message,
                                  std::function<void(const std::string &, bool)> callback) // callback: 对模型返回的增量数据如何处理，第一个参数为增量数据，第二个参数为是否为最后一个增量数据
    {
    }

} // end ai_chat_sdk