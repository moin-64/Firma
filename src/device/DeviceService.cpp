#include "DeviceService.h"
#include <iostream>

DeviceService::DeviceService(DatabaseService* dbService) : dbService(dbService) {
}

DeviceService::~DeviceService() {
}

bool DeviceService::addDevice(const std::string& deviceId, const std::string& deviceType) {
    if (!dbService || !dbService->getConnection()) return false;

    std::string query = "INSERT INTO devices (hostname, type) VALUES ($1, $2)";
    const char* values[2] = {deviceId.c_str(), deviceType.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 2, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool DeviceService::removeDevice(const std::string& deviceId) {
    if (!dbService || !dbService->getConnection()) return false;

    std::string query = "DELETE FROM devices WHERE hostname = $1";
    const char* values[1] = {deviceId.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 1, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

bool DeviceService::assignDeviceToUser(const std::string& deviceId, const std::string& username) {
    if (!dbService || !dbService->getConnection()) return false;

    std::string query = "UPDATE devices SET assigned_user_id = (SELECT id FROM users WHERE username = $1) WHERE hostname = $2";
    const char* values[2] = {username.c_str(), deviceId.c_str()};

    PGresult* res = PQexecParams(dbService->getConnection(), query.c_str(), 2, NULL, values, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

std::vector<Device> DeviceService::getDevices() const {
    std::vector<Device> devices;
    if (!dbService || !dbService->getConnection()) return devices;

    std::string query = "SELECT hostname, type, assigned_user_id FROM devices";
    PGresult* res = PQexec(dbService->getConnection(), query.c_str());

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(res); i++) {
            devices.push_back({
                PQgetvalue(res, i, 0),
                PQgetvalue(res, i, 1),
                PQgetvalue(res, i, 2) // This would be the user_id, not the username
            });
        }
    }

    PQclear(res);
    return devices;
}
