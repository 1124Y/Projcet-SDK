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
        

    
    private:
    private:
        static std::shared_ptr<spdlog::logger> _logger;
        static std::mutex _mutex;
    };

} // end myLog
