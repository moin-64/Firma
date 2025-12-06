#ifndef IDENTITY_SERVICE_H
#define IDENTITY_SERVICE_H

#include <string>
#include <vector>
#include <map>
#include "../core/DatabaseService.h"

struct User {
    std::string username;
    std::string password;
    std::string role;
};

class IdentityService {
public:
    IdentityService(DatabaseService* dbService);
    ~IdentityService();

    bool addUser(const std::string& username, const std::string& password);
    bool removeUser(const std::string& username);
    bool assignRole(const std::string& username, const std::string& role);
    std::vector<User> getUsers() const;

private:
    DatabaseService* dbService;
};

#endif // IDENTITY_SERVICE_H
