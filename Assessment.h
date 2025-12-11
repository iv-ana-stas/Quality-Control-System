#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include <string>
#include <iostream>
#include <map>
#include <memory>
#include "Product.h"
#include "QualityCriterion.h"

namespace QualityControl {

    class Assessment {
    private:
        int id;
        int productId;
        int criterionId;
        double score;
        std::string assessmentDate;
        std::string inspectorName;

    public:
        Assessment();
        Assessment(int id, int pid, int cid, double s, const std::string& date,
            const std::string& inspector);

        int getId() const;
        int getProductId() const;
        int getCriterionId() const;
        double getScore() const;
        std::string getAssessmentDate() const;
        std::string getInspectorName() const;

        void setId(int newId);
        void setScore(int s);
        void setAssessmentDate(const std::string& date);

        static void displayTableHeader();
        static void displayTableFooter();
        void displayInfo() const;
        void displayInfo(const std::map<int, std::shared_ptr<Product>>& products,
            const std::map<int, std::shared_ptr<QualityCriterion>>& criteria) const;

        friend std::ostream& operator<<(std::ostream& os, const Assessment& assessment);
        friend std::istream& operator>>(std::istream& is, Assessment& assessment);
    };

}
#endif
