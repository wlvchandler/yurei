#include <j16/log.h>

namespace logger {
    Logger logInstance("../out/log.txt");

    Logger::Logger(const std::string& filename) {
        logfile.open(filename, std::ios_base::app);
    }

    Logger::~Logger() {
        if (logfile.is_open()) {
            logfile.close();
        }
    }

    void Logger::log(LogLevel level, const std::string& message) {
        std::string levelString;
        switch (level) {
        case LogLevel::DEBUG:
            levelString = "DEBUG";
            break;
        case LogLevel::INFO:
            levelString = "INFO";
            break;
        case LogLevel::WARNING:
            levelString = "WARNING";
            break;
        case LogLevel::ERROR:
            levelString = "ERROR";
            break;
        default:
            levelString = "UNKNOWN";
            break;
        }

        std::time_t now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.erase(timestamp.length() - 1);  // Remove newline

        std::cout << "[" << timestamp << "] " << levelString << ": " << message;
        if (logfile.is_open()) {
            logfile << "[" << timestamp << "] " << levelString << ": " << message;
        }
    }


    LogStream::LogStream(LogLevel level) : level(level) {}

    LogStream dbg(LogLevel::DEBUG);
    LogStream info(LogLevel::INFO);
    LogStream warn(LogLevel::WARNING);
    LogStream err(LogLevel::ERROR);
} 
