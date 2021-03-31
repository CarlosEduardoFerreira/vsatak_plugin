#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>

#include "Poco/AutoPtr.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SimpleFileChannel.h"

#include <android/log.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "globals.hpp"

using Poco::AutoPtr;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::SimpleFileChannel;

namespace Qtoken {

static inline void init_logger(
    const std::string& log_fd, const std::string& logger_name,
    int log_level = Poco::Message::PRIO_INFORMATION,
    const std::string& pattern = "%Y-%m-%d %H:%M:%S %s [%p]: %t") {
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

#ifndef __ANDROID__
static inline void log_message(std::string msg) {
    Poco::Logger::get("GlobalLogger").information(msg);
}
#else
static inline void log_message(std::string msg) {
    __android_log_print(ANDROID_LOG_DEBUG, "### QTOKEN ", "%s", msg.data());
}
#endif

}  // namespace Qtoken

#endif  // LOGGER_H
