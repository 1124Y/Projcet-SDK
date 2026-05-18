#include "../include/DeepSeekProvider.h"
#include "../include/util/myLog.h"
#include <cstdint>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>

namespace ai_chat_sdk
{
    // DeepSeekProvider 类
    bool DeepSeekProvider::initModel(const std::map<std::string, std::string> &modelConfig)
    {
        // 初始化API Key
        auto it = modelConfig.find("api_key");
        if (it == modelConfig.end())
        {
            ERR("DeepSeekProvider initModel api_key not found");
            return false;
        }
        else
        {
            _apiKey = it->second;
        }

        // 初始化Base URL
        it = modelConfig.find("endpoint");
        if (it == modelConfig.end())
        {
            _endpoint = "https://api.deepseek.com";
        }
        else
        {
            _endpoint = it->second;
        }

        _isAvailable = true;
        INFO("DeepSeekProvider initModel success, endpoint: {}", _endpoint);
        return true;
    }

    // 检测模型是否可用
    bool DeepSeekProvider::isAvailable() const
    {
        return _isAvailable;
    }

    // 获取模型名称
    std::string DeepSeekProvider::getModelName() const
    {
        return "deepseek-chat";
    }

    // 获取模型的描述信息
    std::string DeepSeekProvider::getModelDesc() const
    {
        return "一款实用性强、中文优化的通用对话助手，适合日常问答与创作";
    }
}