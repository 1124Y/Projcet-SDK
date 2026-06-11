#include "../include/ChatSDK.h"
#include "../include/util/myLog.h"

namespace ai_chat_sdk
{
    // 初始化模型
    bool ChatSDK::initModels(const std::vector<std::shared_ptr<Config>> &configs)
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
}