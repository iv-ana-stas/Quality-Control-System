#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <map>
#include <memory>
#include <iomanip>
#include "Assessment.h"
#include "Product.h"
#include "QualityCriterion.h"

namespace QualityControl {
    class ReportGenerator {
    private:
        const std::map<int, std::shared_ptr<Assessment>>& assessments;
        const std::map<int, std::shared_ptr<Product>>& products;
        const std::map<int, std::shared_ptr<QualityCriterion>>& criteria;

    public:
        ReportGenerator(const std::map<int, std::shared_ptr<Assessment>>& ass,
            const std::map<int, std::shared_ptr<Product>>& prod,
            const std::map<int, std::shared_ptr<QualityCriterion>>& crit);

        void generateBatchReport(const std::string& batchNumber) const;
    };
}

#endif
