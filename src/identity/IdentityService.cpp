#include "IdentityService.h"
#include <iostream>

IdentityService::IdentityService(DatabaseService* dbService) : dbService(dbService) {
}

IdentityService::~IdentityService() {
}

bool IdentityService::addUser(const std::string& username, const std::string& password) {
    if (!dbService || !dbService->getConnection()) return false;

    // In a real application, hash the password before storing it.
    std::string query = "INSERT INTO users (username, password_hash) VALUES ($1, $2)";
    const char* values[2] = {username.c_str(), password.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 2, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool IdentityService::removeUser(const std::string& username) {
    if (!dbService || !dbService->getConnection()) return false;

    std::string query = "DELETE FROM users WHERE username = $1";
    const char* values[1] = {username.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 1, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool IdentityService::assignRole(const std::string& username, const std::string& role) {
    if (!dbService || !dbService->getConnection()) return false;

    // This is a simplified implementation. A real implementation would look up the role_id.
    std::string query = "UPDATE users SET role_id = (SELECT id FROM roles WHERE name = $1) WHERE username = $2";
    const char* values[2] = {role.c_str(), username.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 2, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

std::vector<User> IdentityService::getUsers() const {
    std::vector<User> users;
    if (!dbService || !dbService->getConnection()) return users;

    std::string query = "SELECT username, role_id FROM users";
    PGresult* res = PQexec(dbService->getConnection(), query.c_str());

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); i++) {
            users.push_back({
                PQgetvalue(res, i, 0),
                "", // Password hash is not returned
                PQgetvalue(res, i, 1) // This would be the role_id, not the role name
            });
        }
    }

    PQclear(res);
    return users;
}
