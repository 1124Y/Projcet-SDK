#pragma once
#include <mutex>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <memory>

namespace myLog
{
    class Logger
    {
    public:
        static void initLogger(const std::string& loggerName, const std::string& loggerFile, spdlog::level::level_enum logLevel = spdlog::level::info);
        static std::shared_ptr<spdlog:: logger> getLogger();
    
    private:
    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

} // end myLog
