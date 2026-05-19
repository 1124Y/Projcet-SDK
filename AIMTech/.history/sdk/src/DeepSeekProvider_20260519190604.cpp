#include "../include/DeepSeekProvider.h"
#include "../include/util/myLog.h"

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
        // INFO("DeepSeekProvider initModel success, endpoint: {}", _endpoint);
        INFO("DeepSeekProvider initModel success, apikey: {}, endpoint: {}", _apiKey, _endpoint);
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

    // 发送消息 - 全量返回     messages: 消息列表，requestParam: 请求参数:模型名称、消息列表、温度值、mantokens最大生成长度、是否开启流式响应（默认是关闭）
    std::string DeepSeekProvider::sendMessage(const std::vector<Message> &messages, const std::map<std::string, std::string> &requestParam)
    {
        // 1. 检测模型是否可用
        if (!isAvailable())
        {
            ERR("DeepSeekProvider sendMessage model not available");
            return;
        }

        // 2. 构造请求参数
        double temperature = 0.7;
        int maxTokens = 2048;
        if (requestParam.find("temperature") != requestParam.end())
        {
            temperature = std::stod(requestParam.at("temperature"));
        }
        if (requestParam.find("max_tokens") != requestParam.end())
        {
            maxTokens = std::stoi(requestParam.at("max_tokens"));
        }
    }
    // 发送消息 - 增量返回 - 流式响应
    std::string DeepSeekProvider::sendMessageStream(const std::vector<Message> &messages,
                                                    const std::map<std::string, std::string> &requestParam,
                                                    std::function<void(const std::string &, bool)> callback)
    {
    }
} // end ai_chat_sdk