#include <iostream>
#include <locale>
#include <windows.h>
#include "QualityApp.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    try {
        QualityControl::QualityApp app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
