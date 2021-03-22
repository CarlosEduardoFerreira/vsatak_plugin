#include "logger.hpp"
#include "globals.hpp"

using Poco::AutoPtr;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::SimpleFileChannel;
using namespace Qtoken;

/**
 * Creates a logger accessible by the logger_name given.
 * @param log_fd      Log file in which to store the logs for this logger
 * @param logger_name The name to access this logger with.
 * @param log_level   The level of logging for this logger, see include.Poco.Poco.Poco::Message
 * docs for priority details. Default = 6
 * @param pattern     The pattern for the log message of this logger. See
 * include.Poco.Poco.Poco::PatternFormatter for formatting details. Default = "Year-month-day
 * Hour:Minute:Second Source [priority]: Text"
 */
void init_logger(const std::string& log_fd, const std::string& logger_name,
                 int log_level, const std::string& pattern) {
    std::string logs_dir;
    const libconfig::Setting& settings = cfg->getRoot();
    const libconfig::Setting& gen_cfgs = settings["file_system"]["general"];

    gen_cfgs.lookupValue("logs_dir", logs_dir);
    std::string full_log_fd = logs_dir + log_fd;

    // using C code here as std::filesystem breaks android cross-compile
    struct stat st = {0};
    if (stat(full_log_fd.data(), &st) == -1) {
        std::ofstream log_file(full_log_fd);
        log_file.close();
    }

    AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pChannel));

    pChannel->setProperty("path", full_log_fd);
    pChannel->setProperty("rotation",
                          "2 K");  // Rotate log file at 2 kilobyte size

    Logger::create(logger_name, pFC, log_level);
}
