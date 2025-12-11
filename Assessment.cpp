#include "Assessment.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>

namespace QualityControl {

    Assessment::Assessment()
        : id(0), productId(0), criterionId(0), score(0.0), assessmentDate(""), inspectorName("") {
    }

    Assessment::Assessment(int id, int pid, int cid, double s, const std::string& date, const std::string& inspector)
        : id(id), productId(pid), criterionId(cid), score(s), assessmentDate(date), inspectorName(inspector) {
    }

    int Assessment::getId() const { return id; }
    int Assessment::getProductId() const { return productId; }
    int Assessment::getCriterionId() const { return criterionId; }
    double Assessment::getScore() const { return score; }
    std::string Assessment::getAssessmentDate() const { return assessmentDate; }
    std::string Assessment::getInspectorName() const { return inspectorName; }

    void Assessment::setId(int newId) { id = newId; }
    void Assessment::setScore(int s) { score = s; }
    void Assessment::setAssessmentDate(const std::string& date) { assessmentDate = date; }

    void Assessment::displayInfo() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << score;
        std::string scoreStr = ss.str();
        std::cout << " "
            << Utils::leftPad(std::to_string(id + 1), 5)
            << Utils::leftPad(std::to_string(productId + 1), 15)
            << Utils::leftPad(std::to_string(criterionId + 1), 15)
            << Utils::leftPad(scoreStr, 10)
            << Utils::leftPad(assessmentDate, 12)
            << Utils::leftPad(inspectorName, 15) << std::endl;
    }

    void Assessment::displayInfo(const std::map<int, std::shared_ptr<Product>>& products,
        const std::map<int, std::shared_ptr<QualityCriterion>>& criteria) const {

        auto prodIt = products.find(productId);
        auto critIt = criteria.find(criterionId);

        std::string prodName = (prodIt != products.end()) ? prodIt->second->getName() : "Unknown";
        std::string critName = (critIt != criteria.end()) ? critIt->second->getName() : "Unknown";
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << score;
        std::string scoreStr = ss.str();
        
        std::cout << " "
            << Utils::leftPad(std::to_string(id + 1), 5)
            << Utils::leftPad((Utils::utf8_strlen(prodName) > 30 ? Utils::utf8_substr(prodName, 0, 19) + "..." : prodName), 35)
            << Utils::leftPad((Utils::utf8_strlen(critName) > 30 ? Utils::utf8_substr(critName, 0, 19) + "..." : critName), 35)
            << Utils::leftPad(scoreStr, 10)
            << Utils::leftPad(assessmentDate, 12)
            << Utils::leftPad(inspectorName, 15) << std::endl;
    }

    void Assessment::displayTableHeader() {
        std::cout << std::string(110, '=') << std::endl;
            std::cout << " "
                << Utils::leftPad("ID", 5)
                << Utils::leftPad("Продукт", 35)
                << Utils::leftPad("Критерий", 32)
                << Utils::leftPad("Оценка", 16)
                << Utils::leftPad("Дата", 9)
                << Utils::leftPad("Инспектор", 15) << std::endl;
        std::cout << std::string(110, '-') << std::endl;
    }

    void Assessment::displayTableFooter() {
        std::cout << std::string(110, '=') << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const Assessment& assessment) {
        os << (assessment.id + 1) << ";" << (assessment.productId + 1) << ";" << (assessment.criterionId + 1)
            << ";" << assessment.score << ";" << assessment.assessmentDate << ";" << assessment.inspectorName;
        return os;
    }

    std::istream& operator>>(std::istream& is, Assessment& assessment) {
        std::string line;
        if (!std::getline(is, line)) return is;
        if (line.empty()) return is;

        std::istringstream iss(line);
        std::string idStr, prodIdStr, critIdStr, scoreStr;

        if (!std::getline(iss, idStr, ';')) return is;

        if (idStr.size() >= 3 &&
            static_cast<unsigned char>(idStr[0]) == 0xEF &&
            static_cast<unsigned char>(idStr[1]) == 0xBB &&
            static_cast<unsigned char>(idStr[2]) == 0xBF) {
            idStr = idStr.substr(3);
        }

        if (!std::getline(iss, prodIdStr, ';')) return is;
        if (!std::getline(iss, critIdStr, ';')) return is;
        if (!std::getline(iss, scoreStr, ';')) return is;

        try {
            assessment.id = std::stoi(idStr) - 1;
        }
        catch (...) {
            return is;
        }

        try {
            assessment.productId = std::stoi(prodIdStr) - 1;
        }
        catch (...) {
            assessment.productId = 0;
        }

        try {
            assessment.criterionId = std::stoi(critIdStr) - 1;
        }
        catch (...) {
            assessment.criterionId = 0;
        }

        try {
            assessment.score = std::stod(scoreStr);
        }
        catch (...) {
            assessment.score = 0.0;
        }

        std::getline(iss, assessment.assessmentDate, ';');
        std::getline(iss, assessment.inspectorName, ';');

        return is;
    }

}
