#include "QualityCriterion.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <wincrypt.h>

namespace QualityControl {

    QualityCriterion::QualityCriterion(int i, const std::string& n, const std::string& desc, double max)
        : id(i), name(n), description(desc), maxScore(max) {
    }

    QualityCriterion::QualityCriterion()
        : id(0), name(""), description(""), maxScore(0.0) {
    }

    int QualityCriterion::getId() const { return id; }
    std::string QualityCriterion::getName() const { return name; }
    std::string QualityCriterion::getDescription() const { return description; }
    double QualityCriterion::getMaxScore() const { return maxScore; }

    void QualityCriterion::setName(const std::string& n) { name = n; }
    void QualityCriterion::setDescription(const std::string& desc) { description = desc; }
    void QualityCriterion::setMaxScore(double max) { maxScore = max; }

    void QualityCriterion::displayInfo() const {
        if (id < 0 || name.empty()) return;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << maxScore;
        std::string maxScoreStr = ss.str();
        std::cout << " "
            << Utils::leftPad(std::to_string(id + 1), 5)
            << Utils::leftPad((Utils::utf8_strlen(name) > 30 ? Utils::utf8_substr(name, 0, 17) + "..." : name), 35)
            << Utils::leftPad(maxScoreStr, 15)
            << Utils::leftPad((Utils::utf8_strlen(description) > 35 ? Utils::utf8_substr(description, 0, 27) + "..." : description), 40) << std::endl;
    }

    void QualityCriterion::displayTableHeader() {
        std::cout << std::string(95, '=') << std::endl;
        std::cout << " "
            << Utils::leftPad("ID", 5)
            << Utils::leftPad("Название", 31)
            << Utils::leftPad("Макс. балл", 19)
            << Utils::leftPad("Описание", 40) << std::endl;
        std::cout << std::string(95, '-') << std::endl;
    }

    void QualityCriterion::displayTableFooter() {
        std::cout << std::string(95, '=') << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const QualityCriterion& criterion) {
        os << (criterion.id + 1) << ";" << criterion.name << ";" << criterion.description << ";" << criterion.maxScore;
        return os;
    }

    std::istream& operator>>(std::istream& is, QualityCriterion& criterion) {
        std::string line;
        if (!std::getline(is, line)) return is;
        if (line.empty()) return is;

        std::istringstream iss(line);
        std::string idStr;

        if (!std::getline(iss, idStr, ';')) return is;

        if (idStr.size() >= 3 &&
            static_cast<unsigned char>(idStr[0]) == 0xEF &&
            static_cast<unsigned char>(idStr[1]) == 0xBB &&
            static_cast<unsigned char>(idStr[2]) == 0xBF) {
            idStr = idStr.substr(3);
        }

        try {
            int fileId = std::stoi(idStr);
            criterion.id = fileId - 1;
        }
        catch (...) {
            return is;
        }

        std::getline(iss, criterion.name, ';');
        std::getline(iss, criterion.description, ';');

        std::string maxScoreStr;
        if (std::getline(iss, maxScoreStr, ';')) {
            try {
                criterion.maxScore = std::stod(maxScoreStr);
            }
            catch (...) {
                criterion.maxScore = 0.0;
            }
        }

        return is;
    }

}
