#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace QualityControl {

	namespace Utils {

		std::string hashPassword(const std::string& password);

		template<typename T>
		T getInput(const std::string& prompt);

		template<>
		std::string getInput<std::string>(const std::string& prompt);

		std::string getCurrentDate();

		size_t utf8_strlen(const std::string& str);
		std::string utf8_substr(const std::string& str, size_t start, size_t length);
		std::string centerText(const std::string& text, size_t width);
		std::string leftPad(const std::string& str, size_t width, char fill = ' ');
		std::string rightPad(const std::string& str, size_t width, char fill = ' ');

	}

}

#endif
