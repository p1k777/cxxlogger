#pragma once

#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace logger {


    // Функция для пользовательского логирования произвольных типов (сигнатура для копирования)
    // void LogType(std::ostream&, const T&);


    // Этот блок нужен для последующей проверки наличия соответствующей специализации предыдущей функции

    template <typename, typename = void>
    struct ExistsLogType : std::false_type {};

    template <typename T>
    struct ExistsLogType<T,
        std::void_t<
            decltype( LogType(std::declval<std::ostream&>(), std::declval<const T&>()) )
        >    
    > : std::true_type {};


    // Базовый класс-логгер, создать не выйдет - конструктор протектед

    class BaseLogger {
    public:

        template <typename T>
        void Custom(const char* log_lvl, const T& val, const char* file_name = nullptr, unsigned line = 0) {
            LogPrefix(log_lvl, file_name, line);
            LogMessage(val);
        }

        template <typename T>
        void Error(const T& val, const char* file_name = nullptr, unsigned line = 0) {
            Custom("ERROR", val, file_name, line);
        }
        template <typename T>
        void Warning(const T& val, const char* file_name = nullptr, unsigned line = 0) {
            Custom("WARNING", val, file_name, line);
        }
        template <typename T>
        void Info(const T& val, const char* file_name = nullptr, unsigned line = 0) {
            Custom("INFO", val, file_name, line);
        }
        template <typename T>
        void Debug(const T& val, const char* file_name = nullptr, unsigned line = 0) {
            Custom("DEBUG", val, file_name, line);
        }

        virtual ~BaseLogger() = default;

    protected:

        BaseLogger(std::ostream* stream_ptr) : stream_ptr_(stream_ptr) {}

        std::ostream* stream_ptr_;


    private:

        void LogPrefix(const char* log_lvl = nullptr, const char* file_name = nullptr, unsigned line = 0) {
            bool f = false;

            if (log_lvl) {
                (*stream_ptr_) << '[' << log_lvl << ']';
                f = true;
            }

            if (file_name && line) {
                (*stream_ptr_) << '[' << file_name << " | " << line << ']';
                f = true;
            }

            if (f) {
                (*stream_ptr_) << ": ";
            }
        }

        template <typename T>
        void LogMessage(const T& val) {
            if constexpr (ExistsLogType<T>::value) {
                LogType((*stream_ptr_), val);
            } else { (*stream_ptr_) << val; }
            (*stream_ptr_) << '\n';
        }
    };


    // Консольный вариант логгера, все очевидно откиснет, если передать НЕ указатель на консольный поток
    class ConsoleLogger final : public BaseLogger {
    public:
        ConsoleLogger(std::ostream* console_stream_ptr) : BaseLogger(console_stream_ptr) {
            if (console_stream_ptr != &std::cout && console_stream_ptr != &std::cerr) {
                // вот тут надо что-то бросить в виде исключения, но пока пусть будет сообщение
                std::cerr << "GOT NOT CONSOLE STREAM POINTER IN 'ConsoleLogger' CONSTRUCTOR!!! PROBABLE FAILURE\n";
            }
        }
    };


    // Файловый вариант логгера, если путь неверный создастся новый файл

    class FileLogger final : public BaseLogger {
    public:
        FileLogger(const std::string& path) : file_(path, std::ios::app), BaseLogger(nullptr) {
            if (!file_.is_open()) {
                // вот тут надо что-то бросить в виде исключения, но пока пусть будет сообщение
                std::cerr << "CAN'T OPEN FILE '" << path << "'!!! PROBABLE FAILURE\n";
            }
            stream_ptr_ = &file_;
        }
        FileLogger(const char* path) : file_(path, std::ios::app), BaseLogger(nullptr) {
            stream_ptr_ = &file_;
        }
    private:
        std::ofstream file_;
    };

} // namespace logger

