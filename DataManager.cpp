#include "DataManager.h"
#include <sstream>
#include <filesystem>
#include <iostream>

namespace QualityControl {

    template<typename T>
    DataManager<T>::DataManager(const std::string& file) {
        std::filesystem::path dataDir = "data";
        if (!std::filesystem::exists(dataDir)) {
            std::filesystem::create_directory(dataDir);
        }
        filename = (dataDir / file).string();
        loadFromFile();
    }

    template<typename T>
    DataManager<T>::~DataManager() {
        saveToFile();
    }

    template<typename T>
    int DataManager<T>::getNextId() {
        if (data.empty()) return 1;
        return data.rbegin()->first + 1;
    }

    template<typename T>
    void DataManager<T>::loadFromFile() {
        data.clear();

        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::istringstream iss(line);
            T item{};
            if (iss >> item) {
                data[item.getId()] = std::make_shared<T>(item);
            }
        }
        file.close();
    }

    template<typename T>
    void DataManager<T>::saveToFile() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }
        for (const auto& pair : data) {
            file << *(pair.second) << std::endl;
        }
        file.close();
    }

    template<typename T>
    void DataManager<T>::add(const std::shared_ptr<T>& item) {
        data[item->getId()] = item;
        saveToFile();
    }

    template<typename T>
    bool DataManager<T>::remove(int id) {
        auto it = data.find(id);
        if (it != data.end()) {
            data.erase(it);
            saveToFile();
            return true;
        }
        return false;
    }

    template<typename T>
    std::shared_ptr<T> DataManager<T>::get(int id) const {
        auto it = data.find(id);
        if (it != data.end()) {
            return it->second;
        }
        return nullptr;
    }

    template<typename T>
    const std::map<int, std::shared_ptr<T>>& DataManager<T>::getAll() const {
        return data;
    }

    template<typename T>
    void DataManager<T>::displayAll() const {
        if (data.empty()) {
            std::cout << "Записей нет." << std::endl;
            return;
        }
        for (const auto& pair : data) {
            pair.second->displayInfo();
        }
    }

    template<>
    void DataManager<Product>::displayAll() const {
        if (data.empty()) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Записей нет " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        Product::displayTableHeader();
        for (const auto& pair : data) {
            pair.second->displayInfo();
        }
        Product::displayTableFooter();
    }

    template<>
    void DataManager<QualityCriterion>::displayAll() const {
        if (data.empty()) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Записей нет " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        QualityCriterion::displayTableHeader();
        for (const auto& pair : data) {
            pair.second->displayInfo();
        }
        QualityCriterion::displayTableFooter();
    }

    template<>
    void DataManager<Assessment>::displayAll() const {
        if (data.empty()) {
            std::cout << std::string(50, '=') << std::endl;
            std::cout << " Записей нет " << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            return;
        }
        Assessment::displayTableHeader();
        for (const auto& pair : data) {
            pair.second->displayInfo();
        }
        Assessment::displayTableFooter();
    }

    DataManager<User>::DataManager(const std::string& file) {
        std::filesystem::path dataDir = "data";
        if (!std::filesystem::exists(dataDir)) {
            std::filesystem::create_directory(dataDir);
        }
        filename = (dataDir / file).string();
        loadFromFile();
    }

    DataManager<User>::~DataManager() {
        saveToFile();
    }

    void DataManager<User>::loadFromFile() {
        data.clear();

        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string username, hashedPwd;
            int roleInt;
            if (std::getline(iss, username, ';') &&
                std::getline(iss, hashedPwd, ';') &&
                (iss >> roleInt)) {
                UserRole role = static_cast<UserRole>(roleInt);
                std::shared_ptr<User> user;
                switch (role) {
                case UserRole::Inspector:
                    user = std::make_shared<Inspector>(username, "dummy_password");
                    break;
                case UserRole::Manager:
                    user = std::make_shared<Manager>(username, "dummy_password");
                    break;
                case UserRole::Admin:
                    user = std::make_shared<Admin>(username, "dummy_password");
                    break;
                }
                user->setHashedPassword(hashedPwd);
                data[username] = user;
            }
        }
        file.close();
    }

    void DataManager<User>::saveToFile() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }
        for (const auto& pair : data) {
            file << *(pair.second) << std::endl;
        }
        file.close();
    }

    void DataManager<User>::add(const std::shared_ptr<User>& user) {
        data[user->getUsername()] = user;
        saveToFile();
    }

    bool DataManager<User>::remove(const std::string& username) {
        auto it = data.find(username);
        if (it != data.end()) {
            data.erase(it);
            saveToFile();
            return true;
        }
        return false;
    }

    std::shared_ptr<User> DataManager<User>::get(const std::string& username) const {
        auto it = data.find(username);
        if (it != data.end()) {
            return it->second;
        }
        return nullptr;
    }

    const std::map<std::string, std::shared_ptr<User>>& DataManager<User>::getAll() const {
        return data;
    }

    void DataManager<User>::displayAll() const {
        if (data.empty()) {
            std::cout << "Пользователей нет." << std::endl;
            return;
        }
        User::displayTableHeader();
        for (const auto& pair : data) {
            pair.second->displayInfo();
        }
        User::displayTableFooter();
    }

    template class DataManager<Product>;
    template class DataManager<QualityCriterion>;
    template class DataManager<Assessment>;

}