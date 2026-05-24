#include <gtest/gtest.h>
#include "../sdk/include/DeepSeekProvider.h"
#include "../sdk/include/util/myLog.h"

TEST(DeepSeekProviderTest, InitModelTest)
{
}

int main(int argc, char **argv)
{
    // 初始化spdlog日志库
    myLog::Logger::initLogger("testLLM", "stdout", spdlog::level::debug);

    // 初始化gtest库
    testing::InitGoogleTest(&argc, argv);

    // 执行所有的测试用例
    return RUN_ALL_TESTS();
}