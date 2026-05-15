#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace myLog
{
    class Logger
    {
    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

} // end myLog
