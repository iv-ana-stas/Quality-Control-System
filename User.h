#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

namespace QualityControl {
    enum class UserRole {
        Inspector,
        Manager,
        Admin
    };

    class User {
    protected:
        std::string username;
        std::string hashedPassword;
        UserRole role;

    public:
        User(const std::string& uname, const std::string& pwd, UserRole r);
        virtual ~User() = default;

        std::string getUsername() const;
        std::string getHashedPassword() const;
        UserRole getRole() const;

        virtual void displayInfo() const;
        static void displayTableHeader();
        static void displayTableFooter();

        virtual std::string getRoleString() const = 0;

        friend std::ostream& operator<<(std::ostream& os, const User& user);

        void setHashedPassword(const std::string& hashedPwd);
    };

    class Inspector : public User {
    public:
        Inspector(const std::string& uname, const std::string& pwd);
        std::string getRoleString() const override;
    };

    class Manager : public User {
    public:
        Manager(const std::string& uname, const std::string& pwd);
        std::string getRoleString() const override;
    };

    class Admin : public User {
    public:
        Admin(const std::string& uname, const std::string& pwd);
        std::string getRoleString() const override;
    };
}

#endif
