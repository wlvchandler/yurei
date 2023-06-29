#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

namespace logger {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class Logger {
    public:
        Logger(const std::string& filename);
        ~Logger();
        void log(LogLevel level, const std::string& message);

    private:
        std::ofstream logfile;
    };

    extern Logger logInstance;

    class LogStream {
    public:
        LogStream(LogLevel level);

        template <typename T>
        LogStream& operator<<(const T& value) {
            logInstance.log(level, stringify(value));
            return *this;
        }

        LogStream& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
            if (manipulator == static_cast<std::ostream & (*)(std::ostream&)>(std::endl)) {
                std::cout << std::endl;
            }
            else {
                std::ostringstream oss;
                oss << manipulator;
                logInstance.log(level, oss.str());
            }
            return *this;
        }

        LogStream& operator<<(std::ios_base& (*manipulator)(std::ios_base&)) {
            if (manipulator == static_cast<std::ios_base & (*)(std::ios_base&)>(std::hex)) {
                std::cout << std::hex;
            }
            else if (manipulator == static_cast<std::ios_base & (*)(std::ios_base&)>(std::dec)) {
                std::cout << std::dec;
            }
            else if (manipulator == static_cast<std::ios_base & (*)(std::ios_base&)>(std::oct)) {
                std::cout << std::oct;
            }
            else {
                std::ostringstream oss;
                oss << manipulator;
                logInstance.log(level, oss.str());
            }
            return *this;
        }

        LogStream& operator<<(const char* value) {
            logInstance.log(level, value);
            return *this;
        }

        LogStream& operator<<(const std::string& value) {
            logInstance.log(level, value);
            return *this;
        }


    private:
        LogLevel level;
        template <typename T>
        std::string stringify(const T& value) {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    };

    extern LogStream dbg;
    extern LogStream info;
    extern LogStream warn;
    extern LogStream err;
} 