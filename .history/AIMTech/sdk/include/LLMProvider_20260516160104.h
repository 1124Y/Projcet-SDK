#include <string>
#include <map>
#include <vector>
#include "common.h"
namespace ai_char_sdk
{
    class LLMProvider
    {
    public:
        // 初始化模型 纯虚函数，必须由派生类实现
        virtual void initModel(const std::map<std::string, std::string> &modelConfig) = 0;
        // 检测模型是否有效
        virtual bool isAvailable() const = 0;
        // 获取模型名称
        virtual std::string getModelName() const = 0;
        // 获取模型描述
        virtual std::string getModelDesc() const = 0;
        // 发送消息 全量返回
        void sendMessage(const std::vector<Message> &message, const std::map<std::string, std::string> &requestParam);

    private:
        bool _isAvailable = false; // 模型是否有效
        std::string _apiKey;       // 模型的API密钥
        std::string _endpoint;     // 模型的API端点URL
    };
} // end ai_char_sdk