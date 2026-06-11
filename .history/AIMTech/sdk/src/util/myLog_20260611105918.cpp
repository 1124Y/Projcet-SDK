#include "../../include/util/myLog.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace myLog
{
    // 静态成员变量初始化：全局唯一的日志器实例
    std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;
    // 静态成员变量初始化：互斥锁，保证多线程初始化安全
    std::mutex Logger::_mutex;

    Logger::Logger()
    {
    }

    //   loggerName  : 日志器的名称，会显示在日志前缀中
    //   loggerFile  : 日志输出文件路径（如 "./app.log"），同时也会输出到控制台
    //   logLevel    : 日志最低输出级别，默认是 info（低于 info 的日志不会被输出）
    void Logger::initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel)
    {
        if (_logger == nullptr)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_logger == nullptr)
            {
                // 设置全局自动刷新级别，当日志级别 ≥ logLevel 时，日志会被立即刷新到文件
                spdlog::flush_on(logLevel);
                // 启用异步日志，即将日志信息存放队列中，有后台线程负责写入
                // 参数1：队列大小，参数2：后台线程数量
                spdlog::init_thread_pool(32768, 1);
                if ("stdout" == loggerFile)
                {
                    // 创建一个带颜色的输出到控制台的日志器
                    _logger = spdlog::stdout_color_mt(loggerName);
                }
                else
                {
                    // 创建一个文件输出的日志器，日志会被写入到指定的文件中
                    _logger = spdlog::basic_logger_mt<spdlog::async_factory>(loggerName, loggerFile);
                }
            }
            // 格式设置
            // [%H:%M:%S] 时分秒
            // [%n] 日志器名称
            // [%-7l] 日志级别，左对齐，宽度为7个字符
            // %v 日志消息
            _logger->set_pattern("[%H:%M:%S][%n][%-4l]%v");
            _logger->set_level(logLevel);
        }
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger()
    {
        return _logger;
    }

} // end myLog