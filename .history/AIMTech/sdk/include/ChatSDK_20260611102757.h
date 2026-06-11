#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "common.h"

namespace ai_char_sdk
{
    class ChatSDK
    {

    public:
        ChatSDK() = default;
        ~ChatSDK() = default;

        void initialize()
        {
            std::cout << "ChatSDK initialized." << std::endl;
        }

        void sendMessage(const std::string &message)
        {
            std::cout << "Sending message: " << message << std::endl;
        }
    };

} // end ai_char_sdk