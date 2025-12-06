#include "DeviceService.h"

DeviceService::DeviceService() {
}

DeviceService::~DeviceService() {
}

bool DeviceService::addDevice(const std::string& deviceId, const std::string& deviceType) {
    if (devices.find(deviceId) != devices.end()) {
        return false; // Device already exists
    }
    devices[deviceId] = {deviceId, deviceType, ""};
    return true;
}

bool DeviceService::removeDevice(const std::string& deviceId) {
    if (devices.find(deviceId) == devices.end()) {
        return false; // Device not found
    }
    devices.erase(deviceId);
    return true;
}

bool DeviceService::assignDeviceToUser(const std::string& deviceId, const std::string& username) {
    if (devices.find(deviceId) == devices.end()) {
        return false; // Device not found
    }
    devices[deviceId].assignedUser = username;
    return true;
}

const std::map<std::string, Device>& DeviceService::getDevices() const {
    return devices;
}
