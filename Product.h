#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>

namespace QualityControl {
    class Product {
    private:
        int id;
        std::string name;
        std::string batchNumber;
        std::string productionDate;

    public:
        Product();
        Product(int i, const std::string& n, const std::string& batch, const std::string& date);

        int getId() const;
        std::string getName() const;
        std::string getBatchNumber() const;
        std::string getProductionDate() const;

        void setName(const std::string& n);
        void setBatchNumber(const std::string& batch);
        void setProductionDate(const std::string& date);

        void displayInfo() const;
        static void displayTableHeader();
        static void displayTableFooter();

        friend std::ostream& operator<<(std::ostream& os, const Product& product);
        friend std::istream& operator>>(std::istream& is, Product& product);
    };
}

#endif
