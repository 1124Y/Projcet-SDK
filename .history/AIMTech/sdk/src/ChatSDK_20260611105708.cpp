#include "../include/ChatSDK.h"
#include "../include/util/myLog.h"

namespace ai_chat_sdk
{
    // 初始化模型
    bool ChatSDK::initModels(const std::vector<std::shared_ptr<Config>> &configs)
    {
        if (_initialized)
        {
            WARN("ChatSDK has already been initialized.");
            return false;
        }

        // 注册和初始化模型提供者
        registerAllProvider(configs);
        initProviders(configs);

        _initialized = true;
        INFO("ChatSDK initialized successfully with %zu models.", _modelConfigs.size());
        return true;
    }

    // 创建会话
    std::string ChatSDK::createSession(const std::string &modelName)
    {
        if (!_initialized)
        {
            ERR("ChatSDK is not initialized. Please call initModels() first.");
            return "";
        }

        if (_modelConfigs.find(modelName) == _modelConfigs.end())
        {
            ERR("Model '%s' is not registered in the SDK.", modelName.c_str());
            return "";
        }

        // 创建新会话
        std::string sessionId = _sessionManager.createSession(modelName);
        INFO("Created new session with ID: %s for model: %s", sessionId.c_str(), modelName.c_str());
        return sessionId;
    }

    // 获取指定会话
    std::shared_ptr<Session> ChatSDK::getSession(const std::string &sessionId)
    {
        if (!_initialized)
        {
            ERR("ChatSDK is not initialized. Please call initModels() first.");
            return nullptr;
        }

        auto session = _sessionManager.getSession(sessionId);
        if (!session)
        {
            WARN("Session with ID '%s' not found.", sessionId.c_str());
            return nullptr;
        }
        return session;
    }

    // 获取所有会话列表
    std::vector<std::string> ChatSDK::getSessionLists() const
    {
        if (!_initialized)
        {
            ERR("ChatSDK is not initialized. Please call initModels() first.");
            return {};
        }
        return _sessionManager.getAllSessionIds();
    }

    // 删除指定会话
    bool ChatSDK::deleteSession(const std::string &sessionId)
    {
        if (!_initialized)
        {
            ERR("ChatSDK is not initialized. Please call initModels() first.");
            return false;
        }
        bool success = _sessionManager.deleteSession(sessionId);
        if (