#include "../include/DeepSeekProvider.h"
#include "../include/util/myLog.h"
#include <jsoncpp/json/json.h>
#include <httplib.h>
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
            return "";
        }

        // 2. 构造请求参数
        double temperature = 0.7;
        int maxTokens = 2048;
        // 检查用户传入的参数集合里，是否包含了 temperature 字段
        if (requestParam.find("temperature") != requestParam.end())
        {
            temperature = std::stod(requestParam.at("temperature"));
        }
        if (requestParam.find("max_tokens") != requestParam.end())
        {
            maxTokens = std::stoi(requestParam.at("max_tokens"));
        }

        // 构造历史消息
        Json::Value messageArray(Json::arrayValue);
        for (const auto &message : messages)
        {
            Json::Value messageJson;
            messageJson["role"] = message._role;
            messageJson["content"] = message._content;
            messageArray.append(messageJson);
        }

        // 3. 构造请求体
        Json::Value requestBody;
        requestBody["model"] = getModelName();
        requestBody["messages"] = messageArray;
        requestBody["temperature"] = temperature;
        requestBody["max_tokens"] = maxTokens;

        // 4. 序列化
        Json::StreamWriterBuilder writerBuilder;
        writerBuilder["indentation"] = "";
        std::string requestBodyStr = Json::writeString(writerBuilder, requestBody);
        INFO("DeepSeekProvider sendMessage requestBody: {}", requestBodyStr.c_str());

        // 5. 使用cpp-httplib库构造HTTP客户端
        httplib::Client client(_endpoint.c_str());
        client.set_connection_timeout(30, 0); // 连接超时时间为30秒
        client.set_read_timeout(60, 0);       // 设置超时时间为60秒

        // 设置请求头
        httplib::Headers headers =
            {
                {"Authorization", "Bearer " + _apiKey},
                {"Content-Type", "application/json"}};

        // 6. 发送POST请求  v1：是为了和open AI保持一致 headers：请求头 requestBodyStr：请求体  application/json：内容
        auto response = client.Post("/v1/chat/completions", headers, requestBodyStr, "application/json");
        if (!response)
        {
            ERR("DeepSeekProvider sendMessage POST request failed");
            return "";
        }
        INFO("DeepSeekProvider sendMessage POST request success, status : {}", response->status);
        INFO("DeepSeekProvider sendMessage POST request success, body : {}", response->body);

        // 检测响应是否成功
        if (response->status != 200)
        {
            return "";
        }

        // 7. 解析响应体
        Json::Value responseBody;
        Json::CharReaderBuilder readerBuilder;
        std::string parseError;
        std::istringstream responseStream(response->body);
        if (Json::parseFromStream(readerBuilder, responseStream, &responseBody, &parseError))
        {
            // 获取message数组
            // responseBody.isMember("choices")：检查根对象里是否存在choices字段（大模型接口的标准返回字段）。
            // responseBody["choices"].isArray()：检查choices是否是数组类型。
            //! responseBody["choices"].empty()：检查数组不为空，避免取空数组下标时越界。
            if (responseBody.isMember("choices") && responseBody["choices"].isArray() && !responseBody["choices"].empty())
            {
                auto choice = responseBody["choices"][0];

                if (choice.isMember("message") && choice["message"].isMember("content"))
                {
                    // choice["message"]["content"].asString();：把 JSON 字符串类型的内容，转换成 C++ 标准的std::string。
                    std::string replyContent = choice["message"]["content"].asString();
                    INFO("DeepSeekProvider response text: {}", replyContent);
                    return replyContent;
                }
            }
        }

        // 8. json解析失败
        ERR("DeepSeekProvider sendMessage POST response body parse failed, error");
        return "deepseek response json parse failed";
    }
    // 发送消息 - 增量返回 - 流式响应
    std::string DeepSeekProvider::sendMessageStream(const std::vector<Message> &messages,
                                                    const std::map<std::string, std::string> &requestParam,
                                                    std::function<void(const std::string &, bool)> callback)
    {
    }
} // end ai_chat_sdk