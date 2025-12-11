#include "ReportGenerator.h"
#include <vector>
#include <algorithm>
#include <iomanip>

namespace QualityControl {

    ReportGenerator::ReportGenerator(const std::map<int, std::shared_ptr<Assessment>>& ass,
        const std::map<int, std::shared_ptr<Product>>& prod,
        const std::map<int, std::shared_ptr<QualityCriterion>>& crit)
        : assessments(ass), products(prod), criteria(crit) {
    }

    void ReportGenerator::generateBatchReport(const std::string& batchNumber) const {
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " ОТЧЕТ ПО ПАРТИИ: " << batchNumber << " " << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        std::vector<std::shared_ptr<Product>> batchProducts;
        for (const auto& pair : products) {
            if (pair.second->getBatchNumber() == batchNumber) {
                batchProducts.push_back(pair.second);
            }
        }
        if (batchProducts.empty()) {
            std::cout << " Продукции с номером партии '" << batchNumber << "' не найдено " << std::endl;
            std::cout << std::string(70, '=') << std::endl;
            return;
        }
        int productCount = 1;
        for (const auto& product : batchProducts) {
            std::cout << " Продукт " << productCount++ << ": " << product->getName()
                << " (ID: " << (product->getId() + 1) << ")" << std::endl;
            bool hasAssessments = false;
            double totalScore = 0.0;
            double maxTotalScore = 0.0;
            std::cout << std::string(70, '-') << std::endl;
            std::cout << " " << std::left << std::setw(40) << "Критерий"
                << std::setw(15) << "Оценка"
                << std::setw(15) << "Макс. балл" << std::endl;
            std::cout << std::string(70, '-') << std::endl;
            for (const auto& assPair : assessments) {
                const auto& assessment = assPair.second;
                if (assessment->getProductId() == product->getId()) {
                    hasAssessments = true;
                    auto critIt = criteria.find(assessment->getCriterionId());
                    if (critIt != criteria.end()) {
                        const auto& criterion = critIt->second;
                        std::string critName = criterion->getName();
                        if (critName.length() > 35) {
                            critName = critName.substr(0, 30) + "...";
                        }
                        std::cout << " " << std::left << std::setw(43) << critName
                            << std::setw(17) << std::fixed << std::setprecision(0) << assessment->getScore()
                            << std::setw(15) << std::fixed << std::setprecision(0) << criterion->getMaxScore() << std::endl;
                        totalScore += assessment->getScore();
                        maxTotalScore += criterion->getMaxScore();
                        std::cout << std::string(70, '-') << std::endl;
                    }
                }
            }
            if (hasAssessments) {
                double percentage = (maxTotalScore > 0) ? (totalScore / maxTotalScore * 100.0) : 0.0;
                std::cout << std::string(70, '-') << std::endl;
                std::cout << " ИТОГО: " << std::fixed << std::setprecision(2) << totalScore
                    << " / " << std::fixed << std::setprecision(2) << maxTotalScore
                    << " (" << std::fixed << std::setprecision(2) << percentage << "%)" << std::endl;
                std::string status;
                if (percentage >= 80.0) {
                    status = "ВЫСОКИЙ";
                }
                else if (percentage >= 60.0) {
                    status = "СРЕДНИЙ";
                }
                else {
                    status = "НИЗКИЙ";
                }
                std::cout << " Статус качества: " << status << std::endl;
                std::cout << std::string(70, '-') << std::endl;
            }
            else {
                std::cout << " Оценки отсутствуют " << std::endl;
                std::cout << std::string(70, '-') << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " СВОДКА ПО ПАРТИИ: " << batchNumber << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " Всего продуктов: " << batchProducts.size() << std::endl;
        int assessedProducts = 0;
        for (const auto& product : batchProducts) {
            bool hasAssessments = false;
            for (const auto& assPair : assessments) {
                if (assPair.second->getProductId() == product->getId()) {
                    hasAssessments = true;
                    break;
                }
            }
            if (hasAssessments) assessedProducts++;
        }
        std::cout << " Продуктов с оценками: " << assessedProducts << " из " << batchProducts.size() << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " КОНЕЦ ОТЧЕТА " << std::endl;
        std::cout << std::string(70, '=') << std::endl << std::endl;
    }

}
