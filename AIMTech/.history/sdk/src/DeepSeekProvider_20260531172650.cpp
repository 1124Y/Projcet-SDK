
#include "../include/DeepSeekProvider.h"
#include "../include/util/myLog.h"
#include <cstdint>
#include <jsoncpp/json/json.h>
#include <httplib.h>
#include <jsoncpp/json/reader.h>

namespace ai_chat_sdk
{
    // DeepSeekProvider 类
    // 初始化模型 initModel
    bool DeepSeekProvider::initModel(const std::map<std::string, std::string> &modelConfig)
    {
        // 初始化API Key
        auto it = modelConfig.find("api_key");
        // 判断是否找到了api_key这个字段，如果没有找到说明用户没有正确配置API Key，模型无法正常工作返回false。
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
        // 判断是否找到endpoint这个字段，如果没有找到说明用户没有配置endpoint，如果找到了就使用用户配置的endpoint。
        if (it == modelConfig.end())
        {
            _endpoint = "https://api.deepseek.com";
        }
        else
        {
            _endpoint = it->second;
        }
        // ModelInfo类的成员变量_isAvailable标记模型已经初始化成功，可以正常使用了。
        _isAvailable = true;
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
        // at() 相比 [] 的好处：如果键不存在，会直接抛出 std::out_of_range 异常。
        // 但在这里，因为前面已经用 find 做了判断，所以进入 if 块时，键一定存在，at() 不会抛出异常，是安全的。
        if (requestParam.find("temperature") != requestParam.end())
        {
            temperature = std::stod(requestParam.at("temperature"));
        }
        if (requestParam.find("max_tokens") != requestParam.end())
        {
            maxTokens = std::stoi(requestParam.at("max_tokens"));
        }

        // 构造历史消息
        // 初始化一个空的 JSON 数组 messageArray，类型为 arrayValue。
        Json::Value messageArray(Json::arrayValue);
        // 遍历 messages 列表（每个元素是包含 _role 和 _content 的消息对象）
        // 每条消息都被转换成一个 JSON 对象：
        for (const auto &message : messages)
        {
            Json::Value messageJson;
            messageJson["role"] = message._role;
            messageJson["content"] = message._content;
            // 用 append 把每个消息对象添加到 messageArray 中，最终形成完整的对话上下文数组
            messageArray.append(messageJson);
        }

        // 3. 构造请求体
        // 初始化根 JSON 对象 requestBody
        Json::Value requestBody;
        requestBody["model"] = getModelName();
        requestBody["messages"] = messageArray;
        requestBody["temperature"] = temperature;
        requestBody["max_tokens"] = maxTokens;
        // 最终生成
        //{
        //   "model": "deepseek-chat",
        //   "messages": [
        //       {"role": "user", "content": "你好"},
        //       {"role": "assistant", "content": "你好，有什么可以帮你的？"}
        //   ],
        //   "temperature": 0.7,
        //   "max_tokens": 2048
        //  }

        // 4. 序列化
        // Json::StreamWriterBuilder 是 JsonCpp 库的序列化工具
        Json::StreamWriterBuilder writerBuilder;
        // 设置缩进为空字符串
        writerBuilder["indentation"] = "";
        std::string requestBodyStr = Json::writeString(writerBuilder, requestBody);
        INFO("DeepSeekProvider sendMessage requestBody: {}", requestBodyStr.c_str());

        // 5. 使用cpp-httplib库构造HTTP客户端
        // httplib::Client 是 cpp-httplib 库的 HTTP 客户端类
        // 传入 DeepSeek API 的 _endpoint（如 https://api.deepseek.com）
        httplib::Client client(_endpoint.c_str());
        client.set_connection_timeout(50, 0); // 设置连接服务器的超时时间为 50 秒
        client.set_read_timeout(60, 0);       // 设置从服务器读取响应的超时时间为 60 秒

        // 设置请求头
        httplib::Headers headers =
            {
                {"Authorization", "Bearer " + _apiKey},
                // 告诉服务器，请求体是 JSON 格式的数据，服务器会按 JSON 解析请求内容
                {"Content-Type", "application/json"}};

        // 6. 发送POST请求  v1：是为了和open AI保持一致 headers：请求头 requestBodyStr：请求体  application/json：内容
        auto response = client.Post("/v1/chat/completions", headers, requestBodyStr, "application/json");
        // 判断 HTTP 请求是否发送失败
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
        Json::Value responseBody;                          // 用来存放解析后的 JSON 数据
        Json::CharReaderBuilder readerBuilder;             // JsonCpp 库中用于创建 JSON 解析器的工厂类，配置解析行为
        std::string parseError;                            // 如果解析失败，会把错误信息存到这个字符串里，方便调试
        std::istringstream responseStream(response->body); // 把 API 返回的响应体字符串，包装成一个输入流，方便 parseFromStream 读取
        if (Json::parseFromStream(readerBuilder, responseStream, &responseBody, &parseError))
        {
            // 获取message数组
            // responseBody.isMember("choices")：检查根对象里是否存在choices字段（大模型接口的标准返回字段）。
            // responseBody["choices"].isArray()：检查choices是否是数组类型。
            //! responseBody["choices"].empty()：检查数组不为空，避免取空数组下标时越界。
            if (responseBody.isMember("choices") && responseBody["choices"].isArray() && !responseBody["choices"].empty())
            {
                auto choice = responseBody["choices"][0]; // 取 choices 数组的第一个元素
                // 检查 choice 对象中是否存在 message 字段，且 message 中是否存在 content 字段
                if (choice.isMember("message") && choice["message"].isMember("content"))
                {
                    // 把 JSON 字符串类型的内容，转换成 C++ 标准的std::string。
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
        // 1. 检测模型是否可用
        if (!isAvailable())
        {
            ERR("DeepSeekProvider sendMessageStream model not available");
            return "";
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

        // 构造历史消息
        Json::Value messageArray(Json::arrayValue);
        for (const auto &message : messages)
        {
            Json::Value messageObject;
            messageObject["role"] = message._role;
            messageObject["content"] = message._content;
            messageArray.append(messageObject);
        }

        // 3. 构造请求体
        Json::Value requestBody;
        requestBody["model"] = getModelName();
        requestBody["messages"] = messageArray;
        requestBody["temperature"] = temperature;
        requestBody["max_tokens"] = maxTokens;
        requestBody["stream"] = true;

        // 4. 序列化
        Json::StreamWriterBuilder writerBuilder;
        writerBuilder["indentation"] = "";
        std::string requestBodyStr = Json::writeString(writerBuilder, requestBody);
        INFO("DeepSeekProvider sendMessageStream requestBody: {}", requestBodyStr);

        // 5. 使用cpp-httplib库构造HTTP客户端
        httplib::Client client(_endpoint.c_str());
        client.set_connection_timeout(30, 0); // 连接超时时间为30秒
        client.set_read_timeout(300, 0);      // 流式响应需要更长的时间，设置超时时间为300秒

        // 设置请求头
        httplib::Headers headers = {
            {"Authorization", "Bearer " + _apiKey},
            {"Content-Type", "application/json"},
            {"Accept", "text/event-stream"}};

        // 流式处理变量
        std::string buffer;        // 接受流式响应的数据块
        bool gotError = false;     // 标记响应是否成功
        std::string errorMsg;      // 错误描述符
        int statusCode = 0;        // 响应状态码
        bool streamFinish = false; // 标记流式响应是否完成
        std::string fullResponse;  // 累积完整的响应

        // 创建请求对象
        httplib::Request req;
        req.method = "POST";
        req.path = "/v1/chat/completions";
        req.headers = headers;
        req.body = requestBodyStr;
        // 设置响应处理器
        req.response_handler = [&](const httplib::Response &res)
        {
            if (res.status != 200)
            {
                gotError = true;
                errorMsg = "HTTP status code: " + std::to_string(res.status);
                return false; // 终止请求
            }
            return true; // 继续接收后续数据
        };

        // 设置数据接收处理器--解析流式响应的每个块的数据
        req.content_receiver = [&](const char *data, size_t len, size_t offset, size_t totalLength)
        {
            // 验证响应头是否错误，如果出错就不需要再继续接收数据
            if (gotError)
            {
                return false;
            }

            // 追加数据到buffer
            buffer.append(data, len);
            INFO("DeepSeekProvider sendMessageStream buffer: {}", buffer);

            // 处理所有的流式响应的数据块，注意：数据块之间是一个\n\n分隔
            size_t pos = 0;
            while ((pos = buffer.find("\n\n")) != std::string::npos)
            {
                // 截取当前找到的数据块
                std::string chunk = buffer.substr(0, pos);
                buffer.erase(0, pos + 2);

                // 解析该块响应数据的中模型返回的有效数据
                // 处理空行和注释，注意：以:开头的行是注释行，需要忽略
                if (chunk.empty() || chunk[0] == ':')
                {
                    continue;
                }

                // 获取模型返回的有效数据
                if (chunk.compare(0, 6, "data: ") == 0)
                {
                    std::string modelData = chunk.substr(6);

                    // 检测是否为结束标记
                    if (modelData == "[DONE]")
                    {
                        callback("", true);
                        streamFinish = true;
                        return true;
                    }

                    // 反序列化
                    Json::Value modelDataJson;
                    Json::CharReaderBuilder reader;
                    std::string errors;
                    std::istringstream modelDataStream(modelData);
                    if (Json::parseFromStream(reader, modelDataStream, &modelDataJson, &errors))
                    {
                        // 模型返回的json格式的数据现在就保存在modelDataJson
                        if (modelDataJson.isMember("choices") &&                      // 判断是否包含choices字段
                            modelDataJson["choices"].isArray() &&                     // 判断choices字段是否是数组类型
                            !modelDataJson["choices"].empty() &&                      // 判断choices数组是否不为空
                            modelDataJson["choices"][0].isMember("delta") &&          // 判断choices数组的第一个元素是否包含delta字段
                            modelDataJson["choices"][0]["delta"].isMember("content")) // 判断delta字段是否包含content字段
                        {
                            std::string content = modelDataJson["choices"][0]["delta"]["content"].asString();
                            // 处理deltaContent，例如追加到fullResponse
                            fullResponse += content;

                            // 将本次解析出的模型返回的有效数据转给调用sendMessageStraem函数的用户使用---callback
                            callback(content, false);
                        }
                    }
                    else
                    {
                        WARN("DeepSeekProvider sendMessageStream parse modelDataJson error: {}", errors);
                    }
                }
            }
            return true;
        };

        // 给模型发送请求
        auto result = client.send(req);
        if (!result)
        {
            // 请求发送失败，出现网络问题，比如DNS解析失败、连接超时
            ERR("Network error {}", to_string(result.error()));
            return "";
        }

        // 确保流式操作正确结束
        if (!streamFinish)
        {
            WARN("stream ended without [DONE] marker");
            callback("", true);
        }

        return fullResponse;
    }
} // end ai_chat_sdk