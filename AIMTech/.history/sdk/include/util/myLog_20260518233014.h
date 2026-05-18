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
        static void initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel = spdlog::level::info);
        static std::shared_ptr<spdlog::logger> getLogger();

    private:
        Logger();
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

// fmt
#define DBG(format, ...) myLog::getLogger()->bebug(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)

} // end myLog
