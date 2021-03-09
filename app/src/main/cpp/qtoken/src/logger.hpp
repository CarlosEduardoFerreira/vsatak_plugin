#ifndef LOGGER_H
#define LOGGER_H
//#include "Poco/AutoPtr.h"
//#include "Poco/FormattingChannel.h"
//#include "Poco/Logger.h"
//#include "Poco/PatternFormatter.h"
//#include "Poco/SimpleFileChannel.h"

#include <filesystem>
#include <fstream>
#include <iostream>

void init_logger(const std::string& log_fd, 
                 const std::string& logger_name, 
                 int log_level = Poco::Message::PRIO_INFORMATION, 
                 const std::string& pattern = "%Y-%m-%d %H:%M:%S %s [%p]: %t");

#endif  // LOGGER_H