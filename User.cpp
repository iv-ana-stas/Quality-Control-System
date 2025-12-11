#include "User.h"
#include "Utils.h"
#include <iomanip>

namespace QualityControl {

    User::User(const std::string& uname, const std::string& pwd, UserRole r)
        : username(uname), role(r) {
        hashedPassword = Utils::hashPassword(pwd);
    }

    std::string User::getUsername() const { return username; }
    std::string User::getHashedPassword() const { return hashedPassword; }
    UserRole User::getRole() const { return role; }

    void User::displayInfo() const {
        std::string roleStr;
        switch (role) {
        case UserRole::Inspector: roleStr = "Инспектор"; break;
        case UserRole::Manager:   roleStr = "Менеджер"; break;
        case UserRole::Admin:     roleStr = "Администратор"; break;
        }
        std::cout << " "
            << Utils::leftPad(username, 20)
            << Utils::leftPad(roleStr, 20) << std::endl;
    }

    void User::displayTableHeader() {
        std::cout << std::string(50, '=') << std::endl;
        std::cout << " "
            << Utils::leftPad("Имя пользователя", 20)
            << Utils::leftPad("Роль", 20) << std::endl;
        std::cout << std::string(50, '-') << std::endl;
    }

    void User::displayTableFooter() {
        std::cout << std::string(50, '=') << std::endl;
    }

    void User::setHashedPassword(const std::string& hashedPwd) {
        hashedPassword = hashedPwd;
    }

    std::ostream& operator<<(std::ostream& os, const User& user) {
        os << user.username << ";" << user.hashedPassword << ";" << static_cast<int>(user.role);
        return os;
    }

    Inspector::Inspector(const std::string& uname, const std::string& pwd)
        : User(uname, pwd, UserRole::Inspector) {
    }

    std::string Inspector::getRoleString() const {
        return "Инспектор";
    }

    Manager::Manager(const std::string& uname, const std::string& pwd)
        : User(uname, pwd, UserRole::Manager) {
    }

    std::string Manager::getRoleString() const {
        return "Менеджер";
    }

    Admin::Admin(const std::string& uname, const std::string& pwd)
        : User(uname, pwd, UserRole::Admin) {
    }

    std::string Admin::getRoleString() const {
        return "Администратор";
    }

}
