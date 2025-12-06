#ifndef DEVICE_SERVICE_H
#define DEVICE_SERVICE_H

#include <string>
#include <vector>
#include <map>

struct Device {
    std::string deviceId;
    std::string deviceType;
    std::string assignedUser;
};

class DeviceService {
public:
    DeviceService();
    ~DeviceService();

    bool addDevice(const std::string& deviceId, const std::string& deviceType);
    bool removeDevice(const std::string& deviceId);
    bool assignDeviceToUser(const std::string& deviceId, const std::string& username);
    const std::map<std::string, Device>& getDevices() const;

private:
    std::map<std::string, Device> devices;
};

#endif // DEVICE_SERVICE_H
