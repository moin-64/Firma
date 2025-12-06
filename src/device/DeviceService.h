#ifndef DEVICE_SERVICE_H
#define DEVICE_SERVICE_H

#include <string>
#include <vector>
#include <map>
#include "../core/DatabaseService.h"

struct Device {
    std::string deviceId;
    std::string deviceType;
    std::string assignedUser;
};

class DeviceService {
public:
    DeviceService(DatabaseService* dbService);
    ~DeviceService();

    bool addDevice(const std::string& deviceId, const std::string& deviceType);
    bool removeDevice(const std::string& deviceId);
    bool assignDeviceToUser(const std::string& deviceId, const std::string& username);
    std::vector<Device> getDevices() const;

private:
    DatabaseService* dbService;
};

#endif // DEVICE_SERVICE_H
