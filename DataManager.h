#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <stdexcept>
#include "User.h"
#include "Product.h"
#include "QualityCriterion.h"
#include "Assessment.h"

namespace QualityControl {
    template<typename T>
    class DataManager {
    protected:
        std::map<int, std::shared_ptr<T>> data;
        std::string filename;

        virtual int getNextId();

    public:
        DataManager(const std::string& file);
        virtual ~DataManager();

        void loadFromFile();
        void saveToFile() const;

        void add(const std::shared_ptr<T>& item);
        bool remove(int id);
        std::shared_ptr<T> get(int id) const;
        const std::map<int, std::shared_ptr<T>>& getAll() const;
        void displayAll() const;
    };

    template<>
    class DataManager<User> {
    private:
        std::map<std::string, std::shared_ptr<User>> data;
        std::string filename;

    public:
        DataManager(const std::string& file);
        ~DataManager();

        void loadFromFile();
        void saveToFile() const;

        void add(const std::shared_ptr<User>& user);
        bool remove(const std::string& username);
        std::shared_ptr<User> get(const std::string& username) const;
        const std::map<std::string, std::shared_ptr<User>>& getAll() const;
        void displayAll() const;
    };
}

#endif
