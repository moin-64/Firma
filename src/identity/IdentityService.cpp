#include "IdentityService.h"

IdentityService::IdentityService() {
}

IdentityService::~IdentityService() {
}

bool IdentityService::addUser(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        return false; // User already exists
    }
    users[username] = {username, password, ""};
    return true;
}

bool IdentityService::removeUser(const std::string& username) {
    if (users.find(username) == users.end()) {
        return false; // User not found
    }
    users.erase(username);
    return true;
}

bool IdentityService::assignRole(const std::string& username, const std::string& role) {
    if (users.find(username) == users.end()) {
        return false; // User not found
    }
    users[username].role = role;
    return true;
}

const std::map<std::string, User>& IdentityService::getUsers() const {
    return users;
}
