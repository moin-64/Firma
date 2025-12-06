#ifndef IDENTITY_SERVICE_H
#define IDENTITY_SERVICE_H

#include <string>
#include <vector>
#include <map>

struct User {
    std::string username;
    std::string password;
    std::string role;
};

class IdentityService {
public:
    IdentityService();
    ~IdentityService();

    bool addUser(const std::string& username, const std::string& password);
    bool removeUser(const std::string& username);
    bool assignRole(const std::string& username, const std::string& role);
    const std::map<std::string, User>& getUsers() const;

private:
    std::map<std::string, User> users;
};

#endif // IDENTITY_SERVICE_H
