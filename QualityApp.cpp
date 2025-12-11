#include "QualityApp.h"
#include "Utils.h"
#include <algorithm>
#include <iomanip>
#include <vector>

namespace QualityControl {

    int QualityApp::nextAssessmentId = 1;

    QualityApp::QualityApp()
        : userManager(std::make_unique<DataManager<User>>("users.txt")),
        productManager(std::make_unique<DataManager<Product>>("products.txt")),
        criterionManager(std::make_unique<DataManager<QualityCriterion>>("criteria.txt")),
        assessmentManager(std::make_unique<DataManager<Assessment>>("assessments.txt")) {


        if (userManager->getAll().empty()) {
            auto admin = std::make_shared<Admin>("admin", "admin123");
            userManager->add(admin);
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Создан администратор по умолчанию: admin / admin123" << std::endl;
            std::cout << std::string(50, '=') << std::endl;
        }
    }

    int QualityApp::getNextAssessmentId() {
        return nextAssessmentId++;
    }

    void QualityApp::run() {
        while (true) {
            if (!currentUser) {
                showLoginMenu();
            }
            else {
                showMainMenu();
            }
        }
    }

    void QualityApp::showLoginMenu() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " СИСТЕМА ОЦЕНКИ КАЧЕСТВА ПРОДУКЦИИ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Вход в систему" << std::endl;
        std::cout << "2. Выход" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: login(); break;
        case 2: std::cout << "До свидания!\n"; exit(0);
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::login() {
        std::string username = Utils::getInput<std::string>("Имя пользователя: ");
        std::string password = Utils::getInput<std::string>("Пароль: ");
        auto user = userManager->get(username);
        if (user && user->getHashedPassword() == Utils::hashPassword(password)) {
            currentUser = user;
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Успешный вход! Добро пожаловать, " << username << "!" << std::endl;
            std::cout << std::string(50, '=') << std::endl;
        }
        else {
            std::cout << "Ошибка: Неверное имя пользователя или пароль.\n";
        }
    }

    void QualityApp::logout() {
        currentUser.reset();
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " Вы вышли из системы. " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    void QualityApp::showMainMenu() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " ГЛАВНОЕ МЕНЮ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Текущий пользователь: " << currentUser->getUsername() << " (" << currentUser->getRoleString() << ")" << std::endl;
        std::cout << "1. Просмотр данных" << std::endl;
        std::cout << "2. Поиск и фильтрация" << std::endl;
        if (currentUser->getRole() == UserRole::Inspector) {
            std::cout << "3. Выставление оценок" << std::endl;
        }
        if (currentUser->getRole() == UserRole::Manager) {
            std::cout << "4. Управление продукцией" << std::endl;
            std::cout << "5. Управление критериями" << std::endl;
            std::cout << "6. Формирование отчета" << std::endl;
        }
        if (currentUser->getRole() == UserRole::Admin || currentUser->getRole() == UserRole::Inspector) {
            std::cout << "7. Управление пользователями" << std::endl;
        }
        std::cout << "0. Выход из системы" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        try {
            switch (choice) {
            case 1: viewData(); break;
            case 2: searchAndFilter(); break;
            case 3: manageAssessments(); break;
            case 4: manageProducts(); break;
            case 5: manageCriteria(); break;
            case 6: generateReport(); break;
            case 7: manageUsers(); break;
            case 0: logout(); break;
            default: std::cout << "Неверный выбор.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Произошла ошибка: " << e.what() << std::endl;
        }
    }

    void QualityApp::viewData() const {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " ПРОСМОТР ДАННЫХ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Просмотр всей продукции" << std::endl;
        std::cout << "2. Просмотр всех критериев" << std::endl;
        std::cout << "3. Просмотр всех оценок" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1:
            std::cout << std::string(90, '=') << std::endl;
            std::cout << " ВСЯ ПРОДУКЦИЯ " << std::endl;
            productManager->displayAll();
            break;
        case 2:
            std::cout << std::string(95, '=') << std::endl;
            std::cout << " ВСЕ КРИТЕРИИ " << std::endl;
            criterionManager->displayAll();
            break;
        case 3:
            std::cout << std::string(110, '=') << std::endl;
            std::cout << " ВСЕ ОЦЕНКИ " << std::endl;
            displayAllAssessmentsWithDetails();
            break;
        case 0:
            break;
        default:
            std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::displayAllAssessmentsWithDetails() const {
        const auto& assessments = assessmentManager->getAll();
        const auto& products = productManager->getAll();
        const auto& criteria = criterionManager->getAll();
        if (assessments.empty()) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Оценок нет " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        Assessment::displayTableHeader();
        for (const auto& pair : assessments) {
            pair.second->displayInfo(products, criteria);
        }
        Assessment::displayTableFooter();
    }

    void QualityApp::manageProducts() {
        if (currentUser->getRole() == UserRole::Inspector) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Недостаточно прав. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " УПРАВЛЕНИЕ ПРОДУКЦИЕЙ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Добавить продукцию" << std::endl;
        std::cout << "2. Редактировать продукцию" << std::endl;
        std::cout << "3. Удалить продукцию" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: addProduct(); break;
        case 2: editProduct(); break;
        case 3: deleteProduct(); break;
        case 0: break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::addProduct() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " ДОБАВЛЕНИЕ ПРОДУКЦИИ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        int id = productManager->getAll().empty() ? 0 : productManager->getAll().rbegin()->first;
        std::string name = Utils::getInput<std::string>("Название продукции: ");
        std::string batch = Utils::getInput<std::string>("Номер партии: ");
        std::string date = Utils::getInput<std::string>("Дата производства (ГГГГ-ММ-ДД): ");
        auto product = std::make_shared<Product>(id, name, batch, date);
        productManager->add(product);
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " Продукция успешно добавлена! " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    void QualityApp::editProduct() {
        int inputId = Utils::getInput<int>("Введите ID продукции для редактирования: ");
        int id = inputId - 1;

        auto product = productManager->get(id);
        if (!product) {
            std::cout << "Продукция с ID " << inputId << " не найдена.\n";
            return;
        }
        std::cout << std::string(90, '=') << std::endl;
        std::cout << " ТЕКУЩИЕ ДАННЫЕ " << std::endl;
        Product::displayTableHeader();
        product->displayInfo();
        Product::displayTableFooter();
        std::string name = Utils::getInput<std::string>("Новое название (оставьте пустым для без изменений): ");
        if (!name.empty()) product->setName(name);
        std::string batch = Utils::getInput<std::string>("Новый номер партии (оставьте пустым для без изменений): ");
        if (!batch.empty()) product->setBatchNumber(batch);
        std::string date = Utils::getInput<std::string>("Новая дата производства (оставьте пустым для без изменений): ");
        if (!date.empty()) product->setProductionDate(date);
        productManager->add(product);
        std::cout << std::string(90, '=') << std::endl;
        std::cout << " Продукция успешно обновлена! " << std::endl;
        std::cout << std::string(90, '=') << std::endl;
    }

    void QualityApp::deleteProduct() {
        int inputId = Utils::getInput<int>("Введите ID продукции для удаления: ");
        int id = inputId - 1;

        if (productManager->remove(id)) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Продукция успешно удалена! " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            auto& assessments = assessmentManager->getAll();
            std::vector<int> toRemove;
            for (const auto& pair : assessments) {
                if (pair.second->getProductId() == id) {
                    toRemove.push_back(pair.first);
                }
            }
            for (int assId : toRemove) {
                assessmentManager->remove(assId);
            }
            if (!toRemove.empty()) {
                std::cout << "Удалено связанных оценок: " << toRemove.size() << std::endl;
            }
        }
        else {
            std::cout << "Продукция с ID " << inputId << " не найдена.\n";
        }
    }

    void QualityApp::manageCriteria() {
        if (currentUser->getRole() == UserRole::Inspector) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Недостаточно прав. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " УПРАВЛЕНИЕ КРИТЕРИЯМИ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Добавить критерий" << std::endl;
        std::cout << "2. Редактировать критерий" << std::endl;
        std::cout << "3. Удалить критерий" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: addCriterion(); break;
        case 2: editCriterion(); break;
        case 3: deleteCriterion(); break;
        case 0: break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::addCriterion() {
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " ДОБАВЛЕНИЕ КРИТЕРИЯ " << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        int id = criterionManager->getAll().empty() ? 0 : criterionManager->getAll().rbegin()->first;
        std::string name = Utils::getInput<std::string>("Название критерия: ");
        std::string desc = Utils::getInput<std::string>("Описание критерия: ");
        double maxScore = Utils::getInput<double>("Максимальный балл: ");
        auto criterion = std::make_shared<QualityCriterion>(id, name, desc, maxScore);
        criterionManager->add(criterion);
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " Критерий успешно добавлен! " << std::endl;
        std::cout << std::string(70, '=') << std::endl;
    }

    void QualityApp::editCriterion() {
        int inputId = Utils::getInput<int>("Введите ID критерия для редактирования: ");
        int id = inputId - 1;

        auto criterion = criterionManager->get(id);
        if (!criterion) {
            std::cout << "Критерий с ID " << inputId << " не найдена.\n";
            return;
        }
        std::cout << std::string(95, '=') << std::endl;
        std::cout << " ТЕКУЩИЕ ДАННЫЕ " << std::endl;
        QualityCriterion::displayTableHeader();
        criterion->displayInfo();
        QualityCriterion::displayTableFooter();
        std::string name = Utils::getInput<std::string>("Новое название (оставьте пустым для без изменений): ");
        if (!name.empty()) criterion->setName(name);
        std::string desc = Utils::getInput<std::string>("Новое описание (оставьте пустым для без изменений): ");
        if (!desc.empty()) criterion->setDescription(desc);
        double maxScore = Utils::getInput<double>("Новый макс. балл (введите -1 для без изменений): ");
        if (maxScore >= 0) criterion->setMaxScore(maxScore);
        criterionManager->add(criterion);
        std::cout << std::string(95, '=') << std::endl;
        std::cout << " Критерий успешно обновлен! " << std::endl;
        std::cout << std::string(95, '=') << std::endl;
    }

    void QualityApp::deleteCriterion() {
        int inputId = Utils::getInput<int>("Введите ID критерия для удаления: ");
        int id = inputId - 1;

        if (criterionManager->remove(id)) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Критерий успешно удален! " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            auto& assessments = assessmentManager->getAll();
            std::vector<int> toRemove;
            for (const auto& pair : assessments) {
                if (pair.second->getCriterionId() == id) {
                    toRemove.push_back(pair.first);
                }
            }
            for (int assId : toRemove) {
                assessmentManager->remove(assId);
            }
            if (!toRemove.empty()) {
                std::cout << "Удалено связанных оценок: " << toRemove.size() << std::endl;
            }
        }
        else {
            std::cout << "Критерий с ID " << inputId << " не найден.\n";
        }
    }

    void QualityApp::manageAssessments() {
        if (currentUser->getRole() != UserRole::Inspector &&
            currentUser->getRole() != UserRole::Manager) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Недостаточно прав. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " УПРАВЛЕНИЕ ОЦЕНКАМИ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Выставить новую оценку" << std::endl;
        std::cout << "2. Редактировать оценку" << std::endl;
        std::cout << "3. Удалить оценку" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: addAssessment(); break;
        case 2: editAssessment(); break;
        case 3: deleteAssessment(); break;
        case 0: break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::addAssessment() {
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " ВЫСТАВЛЕНИЕ НОВОЙ ОЦЕНКИ " << std::endl;
        std::cout << std::string(110, '=') << std::endl;
        std::cout << "Доступная продукция:" << std::endl;
        productManager->displayAll();
        int inputProductId = Utils::getInput<int>("Введите ID продукции: ");
        int productId = inputProductId - 1;

        auto product = productManager->get(productId);
        if (!product) {
            std::cout << "Продукция с ID " << inputProductId << " не найдена.\n";
            return;
        }
        std::cout << "Доступные критерии:" << std::endl;
        criterionManager->displayAll();
        int inputCriterionId = Utils::getInput<int>("Введите ID критерия: ");
        int criterionId = inputCriterionId - 1;

        auto criterion = criterionManager->get(criterionId);
        if (!criterion) {
            std::cout << "Критерий с ID " << inputCriterionId << " не найден.\n";
            return;
        }
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << criterion->getMaxScore();
        double score = Utils::getInput<double>("Введите оценку (0 - " + ss.str() + "): ");
        if (score < 0 || score > criterion->getMaxScore()) {
            std::cout << "Ошибка: Оценка должна быть в диапазоне от 0 до " << criterion->getMaxScore() << ".\n";
            return;
        }
        std::string date = Utils::getInput<std::string>("Дата оценки (ГГГГ-ММ-ДД) [по умолчанию: сегодня]: ");
        if (date.empty()) {
            date = Utils::getCurrentDate();
        }
        int id = assessmentManager->getAll().empty() ? 0 : assessmentManager->getAll().rbegin()->first;
        auto assessment = std::make_shared<Assessment>(id, productId, criterionId, score, date, currentUser->getUsername());
        assessmentManager->add(assessment);
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " Оценка успешно добавлена! " << std::endl;
        std::cout << std::string(110, '=') << std::endl;
    }

    void QualityApp::editAssessment() {
        int inputId = Utils::getInput<int>("Введите ID оценки для редактирования: ");
        int id = inputId - 1;

        auto assessment = assessmentManager->get(id);
        if (!assessment) {
            std::cout << "Оценка с ID " << inputId << " не найдена.\n";
            return;
        }
        if (assessment->getInspectorName() != currentUser->getUsername() &&
            currentUser->getRole() != UserRole::Manager &&
            currentUser->getRole() != UserRole::Admin) {
            std::cout << std::string(110, '=') << std::endl;
            std::cout << " Доступ запрещен. Вы можете редактировать только свои оценки. " << std::endl;
            std::cout << std::string(110, '=') << std::endl;
            return;
        }
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " ТЕКУЩИЕ ДАННЫЕ ОЦЕНКИ " << std::endl;
        Assessment::displayTableHeader();
        assessment->displayInfo(productManager->getAll(), criterionManager->getAll());
        Assessment::displayTableFooter();
        int newScore = Utils::getInput<double>("Новая оценка (введите -1 для без изменений): ");
        if (newScore >= 0) {
            auto criterion = criterionManager->get(assessment->getCriterionId());
            if (criterion && newScore <= criterion->getMaxScore()) {
                assessment->setScore(newScore);
            }
            else {
                std::cout << "Ошибка: Новая оценка превышает максимальный балл для данного критерия.\n";
                return;
            }
        }
        std::string newDate = Utils::getInput<std::string>("Новая дата (оставьте пустым для без изменений): ");
        if (!newDate.empty()) {
            assessment->setAssessmentDate(newDate);
        }
        assessmentManager->add(assessment);
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " Оценка успешно обновлена! " << std::endl;
        std::cout << std::string(110, '=') << std::endl;
    }

    void QualityApp::deleteAssessment() {
        int inputId = Utils::getInput<int>("Введите ID оценки для удаления: ");
        int id = inputId - 1;

        auto assessment = assessmentManager->get(id);
        if (!assessment) {
            std::cout << "Оценка с ID " << inputId << " не найдена.\n";
            return;
        }
        if (assessment->getInspectorName() != currentUser->getUsername() &&
            currentUser->getRole() != UserRole::Manager &&
            currentUser->getRole() != UserRole::Admin) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Вы можете удалять только свои оценки. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        if (assessmentManager->remove(id)) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Оценка успешно удалена! " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
        }
        else {
            std::cout << "Ошибка при удалении оценки.\n";
        }
    }

    void QualityApp::searchAndFilter() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " ПОИСК И ФИЛЬТРАЦИЯ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Поиск продукции по названию" << std::endl;
        std::cout << "2. Фильтрация оценок по продукту" << std::endl;
        std::cout << "3. Фильтрация оценок по критерию" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: searchProductsByName(); break;
        case 2: filterAssessmentsByProduct(); break;
        case 3: filterAssessmentsByCriterion(); break;
        case 0: break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::searchProductsByName() {
        std::string query = Utils::getInput<std::string>("Введите название или часть названия для поиска: ");
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);

        const auto& products = productManager->getAll();
        bool found = false;
        for (const auto& pair : products) {
            std::string name = pair.second->getName();
            std::string lowerName = name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(query) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Продукция по запросу \"" << query << "\" не найдена.\n";
            return;
        }
        std::cout << std::string(90, '=') << std::endl;
        std::cout << " РЕЗУЛЬТАТЫ ПОИСКА: \"" << query << "\"\n";
        Product::displayTableHeader();
        for (const auto& pair : products) {
            std::string name = pair.second->getName();
            std::string lowerName = name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(query) != std::string::npos) {
                pair.second->displayInfo();
            }
        }
        Product::displayTableFooter();
    }

    void QualityApp::filterAssessmentsByProduct() {
        int inputProductId = Utils::getInput<int>("Введите ID продукции для фильтрации оценок: ");
        int productId = inputProductId - 1;

        auto product = productManager->get(productId);
        if (!product) {
            std::cout << "Продукция с ID " << inputProductId << " не найдена.\n";
            return;
        }
        const auto& assessments = assessmentManager->getAll();
        bool found = false;
        for (const auto& pair : assessments) {
           if (pair.second->getProductId() == productId) {
               found = true;
               break;
            }
        }
        if (!found) {
           std::cout << "Оценки для данной продукции отсутствуют.\n";
           return;
        }
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " ОЦЕНКИ ДЛЯ ПРОДУКЦИИ: " << product->getName() << "\n";
        Assessment::displayTableHeader();
        for (const auto& pair : assessments) {
            if (pair.second->getProductId() == productId) {
                pair.second->displayInfo(productManager->getAll(), criterionManager->getAll());
            }
        }
        Assessment::displayTableFooter();
    }

    void QualityApp::filterAssessmentsByCriterion() {
        int inputCriterionId = Utils::getInput<int>("Введите ID критерия для фильтрации оценок: ");
        int criterionId = inputCriterionId - 1;

        auto criterion = criterionManager->get(criterionId);
        if (!criterion) {
            std::cout << "Критерий с ID " << inputCriterionId << " не найден.\n";
            return;
        }
        const auto& assessments = assessmentManager->getAll();
        bool found = false;
        for (const auto& pair : assessments) {
            if (pair.second->getCriterionId() == criterionId) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Оценки по критерию \"" << criterion->getName() << "\" отсутствуют.\n";
            return;
        }
        std::cout << std::string(110, '=') << std::endl;
        std::cout << " ОЦЕНКИ ПО КРИТЕРИЮ: " << criterion->getName() << "\n";
        Assessment::displayTableHeader();
        for (const auto& pair : assessments) {
            if (pair.second->getCriterionId() == criterionId) {
                pair.second->displayInfo(productManager->getAll(), criterionManager->getAll());
            }
        }
        Assessment::displayTableFooter();
    }

    void QualityApp::generateReport() {
        if (currentUser->getRole() != UserRole::Manager &&
            currentUser->getRole() != UserRole::Admin) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Недостаточно прав. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        std::cout << std::string(70, '=') << std::endl;
        std::cout << " ФОРМИРОВАНИЕ ОТЧЕТА " << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        std::string batchNumber = Utils::getInput<std::string>("Введите номер партии для отчета: ");
        ReportGenerator reporter(assessmentManager->getAll(),
            productManager->getAll(),
            criterionManager->getAll());
        reporter.generateBatchReport(batchNumber);
    }

    void QualityApp::manageUsers() {
        if (currentUser->getRole() != UserRole::Admin) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Доступ запрещен. Недостаточно прав. " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " УПРАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯМИ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Добавить пользователя" << std::endl;
        std::cout << "2. Удалить пользователя" << std::endl;
        std::cout << "3. Просмотреть всех пользователей" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Выберите опцию: ";
        int choice = Utils::getInput<int>("");
        switch (choice) {
        case 1: addUser(); break;
        case 2: deleteUser(); break;
        case 3: userManager->displayAll(); break;
        case 0: break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void QualityApp::addUser() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " ДОБАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯ " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::string username = Utils::getInput<std::string>("Имя пользователя: ");
        if (userManager->get(username)) {
            std::cout << "Ошибка: Пользователь с таким именем уже существует.\n";
            return;
        }
        std::string password = Utils::getInput<std::string>("Пароль: ");
        std::cout << "Выберите роль:" << std::endl;
        std::cout << "1. Инспектор" << std::endl;
        std::cout << "2. Менеджер" << std::endl;
        std::cout << "3. Администратор" << std::endl;
        int roleChoice = Utils::getInput<int>("Ваш выбор: ");
        std::shared_ptr<User> newUser;
        switch (roleChoice) {
        case 1:
            newUser = std::make_shared<Inspector>(username, password);
            break;
        case 2:
            newUser = std::make_shared<Manager>(username, password);
            break;
        case 3:
            newUser = std::make_shared<Admin>(username, password);
            break;
        default:
            std::cout << "Неверный выбор роли.\n";
            return;
        }
        userManager->add(newUser);
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " Пользователь успешно добавлен! " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    void QualityApp::deleteUser() {
        std::string username = Utils::getInput<std::string>("Введите имя пользователя для удаления: ");
        if (username == currentUser->getUsername()) {
            std::cout << "Ошибка: Вы не можете удалить сами себя.\n";
            return;
        }
        if (userManager->remove(username)) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Пользователь успешно удален! " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
        }
        else {
            std::cout << "Пользователь с именем '" << username << "' не найден.\n";
        }
    }
}