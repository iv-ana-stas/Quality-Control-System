#include "Utils.h"
#include <iostream>
#include <limits>

namespace QualityControl {

    namespace Utils {

        std::string hashPassword(const std::string& password) {
            std::hash<std::string> hasher;
            return std::to_string(hasher(password));
        }

        template<typename T>
        T getInput(const std::string& prompt) {
            T value{};
            while (true) {
                std::cout << prompt;
                if (std::cin >> value) {
                    break;
                }
                else {
                    std::cout << "Ошибка ввода. Пожалуйста, введите корректное значение.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        template<>
        std::string getInput<std::string>(const std::string& prompt) {
            std::string value;
            std::cout << prompt;
            std::getline(std::cin, value);
            return value;
        }

        std::string getCurrentDate() {
            std::time_t t = std::time(nullptr);
            std::tm tm;
            localtime_s(&tm, &t);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d");
            return oss.str();
        }

        size_t utf8_strlen(const std::string& str) {
            size_t len = 0;
            for (unsigned char c : str) {
                if ((c & 0xC0) != 0x80) ++len;
            }
            return len;
        }

        std::string utf8_substr(const std::string& str, size_t start, size_t length) {
            if (start >= utf8_strlen(str)) return "";
            size_t byte_start = 0;
            size_t char_count = 0;
            for (size_t i = 0; i < str.length(); i++) {
                if (char_count == start) {
                    byte_start = i;
                    break;
                }
                if ((str[i] & 0xC0) != 0x80) ++char_count;
            }
            size_t byte_end = byte_start;
            char_count = 0;
            for (size_t i = byte_start; i < str.length() && char_count < length; i++) {
                byte_end = i + 1;
                if ((str[i] & 0xC0) != 0x80) ++char_count;
            }
            return str.substr(byte_start, byte_end - byte_start);
        }

        std::string centerText(const std::string& text, size_t width) {
            size_t len = utf8_strlen(text);
            int padding = std::max(0, static_cast<int>(width) - static_cast<int>(len));
            int left = padding / 2;
            int right = padding - left;
            return std::string(left, ' ') + text + std::string(right, ' ');
        }

        std::string leftPad(const std::string& str, size_t width, char fill) {
            size_t utf8_len = utf8_strlen(str);
            if (utf8_len >= width) return str;
            return str + std::string(width - utf8_len, fill);
        }

        std::string rightPad(const std::string& str, size_t width, char fill) {
            size_t utf8_len = utf8_strlen(str);
            if (utf8_len >= width) return str;
            return std::string(width - utf8_len, fill) + str;
        }

        template int getInput<int>(const std::string& prompt);
        template double getInput<double>(const std::string& prompt);

    }

}
