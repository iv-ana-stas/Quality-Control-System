#ifndef QUALITY_APP_H
#define QUALITY_APP_H

#include <memory>
#include "DataManager.h"
#include "ReportGenerator.h"

namespace QualityControl {
    class QualityApp {
    private:
        std::unique_ptr<DataManager<User>> userManager;
        std::unique_ptr<DataManager<Product>> productManager;
        std::unique_ptr<DataManager<QualityCriterion>> criterionManager;
        std::unique_ptr<DataManager<Assessment>> assessmentManager;

        std::shared_ptr<User> currentUser;
        static int nextAssessmentId;

    public:
        QualityApp();
        static int getNextAssessmentId();
        void run();

    private:
        void showLoginMenu();
        void login();
        void logout();
        void showMainMenu();
        void viewData() const;
        void displayAllAssessmentsWithDetails() const;
        void manageProducts();
        void addProduct();
        void editProduct();
        void deleteProduct();
        void manageCriteria();
        void addCriterion();
        void editCriterion();
        void deleteCriterion();
        void manageAssessments();
        void addAssessment();
        void editAssessment();
        void deleteAssessment();
        void searchAndFilter();
        void searchProductsByName();
        void filterAssessmentsByProduct();
        void filterAssessmentsByCriterion();
        void generateReport();
        void manageUsers();
        void addUser();
        void deleteUser();
    };
}

#endif
