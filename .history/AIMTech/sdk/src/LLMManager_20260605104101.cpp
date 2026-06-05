#include "../include/LLMManager.h"
#include "../include/util/myLog.h"
#include "../include/common.h"

namespace ai_chat_sdk
{
    // 注册LLM提供者
    bool LLMManager::registerProvider(const std::string &modelName, std::unique_ptr<LLMProvider> provider)
    {
        // 参数检测
        if (!provider)
        {
            ERR("cannot register nullptr provider, modelName = {}", modelName);
            return false;
        }

        // 注意：unique_ptr是防拷贝的，此处只能通过move的方式将资源转移给当前对象
        _providers[modelName] = std::move(provider);

        // 添加模型信息
        _modelInfos[modelName] = ModelInfo(modelName);

        // 模型初始化成功
        INFO("register provider success, modelName = {}", modelName);
        return true;
    }

}