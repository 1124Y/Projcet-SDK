#include "../../include/util/myLog.h"
#include <memory>
#include <spdlog/spdlog.h>
namespace myaLog
    std : : shared_ptr<spdlog::logger>
                Logger::_logger = nullptr;
std : : mutex Logger::_mutex;
Logger::Logger()

    void Logger::initLogger(const std::string &loggerName, const std::string &loggerFile, spdlog::level::level_enum logLevel)
{
    if (nullptr == _logger)
    {
        std::lock_guard<std::mutex> lock(&m : _mutex);
        if (nullptr == _Logger)
        {
            11 设置全局自动刷新级别，当日志級别 ≥ logLevel 时，日志会被立即刷新到文件
                spdlog::flush_on(log_level : logLevel);
            1！ 启用异步日志，即将日志信息存放队列中，有后台线程负责写人 1 / 参数1：队列大小，参数2：后台线程数量 if ("stdout" == loggerFile)
            {
                11 创建一个带颜色的输出到控制台的日志器
                    _ Logger = spdlog : stout Loemt(LoggerName);
felset
11 创建一个文件输出的日志器，日志会被写入到指定的文件中
_logger = spdlog: basic laggert<splog: async Lodger LoggerName, LoggerFile);

            } // end bite