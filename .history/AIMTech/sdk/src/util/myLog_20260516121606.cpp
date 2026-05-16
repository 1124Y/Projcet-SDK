#include "../../include/util/myLog.h"

namespace myLog
{
    std::shared_ptr<spdlog::logger> Logger::_logger = nullptr; // 定义静态成员变量
    std::mutex Logger::_mutex;                                 // 定义静态成员变量

    void Logger::initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel)
    {
        if (nullptr == _logger)
        {
        }
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger()
    {
        return _logger;
    }

} // end myLog