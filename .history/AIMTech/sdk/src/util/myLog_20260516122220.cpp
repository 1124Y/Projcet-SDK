#include "../../include/util/myLog.h"
#include <mutex>
namespace myLog
{
    std::shared_ptr<spdlog::logger> Logger::_logger = nullptr; // 定义静态成员变量
    std::mutex Logger::_mutex;                                 // 定义静态成员变量

    void Logger::initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel)
    {
        if (nullptr == _logger)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (nullptr == _logger)
            {
                // 设置全局自动刷新级别，当日志级别 ≥ logger 时，日志会立即刷新到文件
                spdlog::flush_on(logLevel);
            }
        }
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger()
    {
        return _logger;
    }

} // end myLog