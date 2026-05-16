#include <string>

namespace ai_char_sdk
{
    class LLMProvider
    {
    public:
        virtual ~LLMProvider() = default;

        // 纯虚函数，必须由派生类实现
        virtual std::string getModelName() const = 0;
    };
} // end ai_char_sdk