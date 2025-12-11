#ifndef QUALITY_CRITERION_H
#define QUALITY_CRITERION_H

#include <string>
#include <iostream>

namespace QualityControl {
    class QualityCriterion {
    private:
        int id;
        std::string name;
        std::string description;
        double maxScore;

    public:
        QualityCriterion();
        QualityCriterion(int i, const std::string& n, const std::string& desc, double max);

        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
        double getMaxScore() const;

        void setName(const std::string& n);
        void setDescription(const std::string& desc);
        void setMaxScore(double max);

        void displayInfo() const;
        static void displayTableHeader();
        static void displayTableFooter();

        friend std::ostream& operator<<(std::ostream& os, const QualityCriterion& criterion);
        friend std::istream& operator>>(std::istream& is, QualityCriterion& criterion);
    };
}

#endif
