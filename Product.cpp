#include "Product.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>

namespace QualityControl {

    Product::Product() : id(0), name(""), batchNumber(""), productionDate("") {}

    Product::Product(int i, const std::string& n, const std::string& batch, const std::string& date)
        : id(i), name(n), batchNumber(batch), productionDate(date) {
    }

    int Product::getId() const { return id; }
    std::string Product::getName() const { return name; }
    std::string Product::getBatchNumber() const { return batchNumber; }
    std::string Product::getProductionDate() const { return productionDate; }

    void Product::setName(const std::string& n) { name = n; }
    void Product::setBatchNumber(const std::string& batch) { batchNumber = batch; }
    void Product::setProductionDate(const std::string& date) { productionDate = date; }

    void Product::displayInfo() const {
        if (id < 0 || name.empty()) return;

        std::cout << " "
            << Utils::leftPad(std::to_string(id + 1), 4) << " "
            << Utils::leftPad((Utils::utf8_strlen(name) > 35 ? Utils::utf8_substr(name, 0, 32) + "..." : name), 40)
            << Utils::leftPad(batchNumber, 20)
            << Utils::leftPad(productionDate, 20) << std::endl;
    }

    void Product::displayTableHeader() {
        std::cout << std::string(90, '=') << std::endl;
        std::cout << " "
            << Utils::leftPad("ID", 4) << " "
            << Utils::leftPad("Название", 43)
            << Utils::leftPad("Партия", 13)
            << Utils::leftPad("Дата производства", 20) << std::endl;
        std::cout << std::string(90, '-') << std::endl;
    }

    void Product::displayTableFooter() {
        std::cout << std::string(90, '=') << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const Product& product) {
        os << (product.id + 1) << ";" << product.name << ";" << product.batchNumber << ";" << product.productionDate;
        return os;
    }

    std::istream& operator>>(std::istream& is, Product& product) {
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
            product.id = fileId - 1;
        }
        catch (...) {
            return is;
        }

        std::getline(iss, product.name, ';');
        std::getline(iss, product.batchNumber, ';');
        std::getline(iss, product.productionDate, ';');

        return is;
    }

}
