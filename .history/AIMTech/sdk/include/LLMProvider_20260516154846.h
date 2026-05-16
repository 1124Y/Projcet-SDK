#include <string>
#include <map>
namespace ai_char_sdk
{
    class LLMProvider
    {
    public:
        virtual void initModel(const std::map<std::string, std::string> &modelConfig) = 0;
        // 纯虚函数，必须由派生类实现
        virtual std::string getModelName() const = 0;
        virtual std::string getModelDesc() const = 0;
        virtual bool isAvailable() const = 0;

    private:
        bool _isAvailable = false; // 模型是否有效
    };
} // end ai_char_sdk