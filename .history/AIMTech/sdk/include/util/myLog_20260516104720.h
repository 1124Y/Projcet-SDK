#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace myLog
{
    class Logger
    {
    private:
        Logger();
        Logger(const Logger &) = delete;            // 禁止拷贝构造
        Logger &operator=(const Logger &) = delete; // 禁止赋值操作

    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

} // end myLog
