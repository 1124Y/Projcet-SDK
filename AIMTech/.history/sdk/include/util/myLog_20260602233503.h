#pragma once
#include <mutex>
#include <spdlog/logger.h>
#include <mutex>
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <memory>

namespace myLog
{
    class Logger
    {
    public:
        //   loggerName  : 日志器的名称，会显示在日志前缀中
        //   loggerFile  : 日志输出文件路径（如 "./app.log"），同时也会输出到控制台
        //   logLevel    : 日志最低输出级别，默认是 info（低于 info 的日志不会被输出）
        static void initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel = spdlog::level::info);
        static std::shared_ptr<spdlog::logger> getLogger();

    private:
        Logger();
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete; // 禁止赋值操作
        Logger(const Logger &) = delete;            // 禁止拷贝构造

    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

    // fmt
    //__FILE__：C++ 预定义宏，自动替换为当前代码文件的路径/名称 __LINE__：C++ 预定义宏，自动替换为当前代码的行号 ##__VA_ARGS__：处理可变参数的特殊语法
#define TRACE(format, ...) myLog::Logger::getLogger()->trace(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define DBG(format, ...) myLog::Logger::getLogger()->debug(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(format, ...) myLog::Logger::getLogger()->info(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(format, ...) myLog::Logger::getLogger()->warn(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define ERR(format, ...) myLog::Logger::getLogger()->error(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define CRIT(format, ...) myLog::Logger::getLogger()->critical(std::string("[{:>10s}:{:<4d}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
} // end myLog
