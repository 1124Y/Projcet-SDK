#include "../../include/util/myLog.h"

namespace myLog
{
    std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;
    std::mutex Logger::_mutex;

    void Logger::initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel)
    {
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger()
    {
        return _logger;
    }

} // end myLog