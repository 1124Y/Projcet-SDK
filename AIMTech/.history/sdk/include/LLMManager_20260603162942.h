#pragma once

#include <map>
#include <memory>
#include "LLMManager.h"

namespace ai_chat_sdk
{
    class LLMManager
    {

    public:
    private:
        std::map<std::string, std::shared_ptr<LLMProvider>> _providers; // 模型提供者列表，key为模型名称，value为模型提供者对象
    };
}