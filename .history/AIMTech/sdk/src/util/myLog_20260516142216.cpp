#include "../../include/util/myLog.h"
#include <mutex>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

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
                // 创建一个异步日志器，使用默认的线程池和日志格式
                // 参数1：队列大小 参数2：后台线程数量
                spdlog::init_thread_pool(32768, 1);
                if ("strout" == loggerFile)
                {
                    // 创建一个带颜色的输出到控制台的日志器
                    _logger = spdlog::stdout_color_mt(loggerName);
                }
                else
                {
                    // 创建一个文件输出日志器，日志会被写入到指定文件中
                    _logger = spdlog::basic_logger_mt<spdlog::async_logger>(loggerName, loggerFile);
                }
            }

            // 格式设置
            //[%Y-%m-%d %H:%M:%S] 表示时间戳，格式为年-月-日 时:分:秒
            //[%n] 表示日志器的名称
            //[%-7l] 表示日志级别，左对齐，占7个字符
            //%v 表示日志消息的内容
            _logger->set_pattern("[%Y-%m-%d %H:%M:%S][%n][%-7l]%v");
            _logger->set_level(logLevel);
        }
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger()
    {
        return _logger;
    }

} // end myLog


spdlog::t