#include "DeploymentService.h"
#include <iostream>
#include <algorithm>

DeploymentService::DeploymentService(const IdentityService& identityService, const DeviceService& deviceService)
    : identityService(identityService), deviceService(deviceService) {
}

DeploymentService::~DeploymentService() {
}

bool DeploymentService::deployImage(const std::string& deviceId) {
    const auto& devices = deviceService.getDevices();
    auto device_it = std::find_if(devices.begin(), devices.end(), [&](const Device& d) {
        return d.deviceId == deviceId;
    });

    if (device_it == devices.end()) {
        std::cerr << "Deployment failed: Device not found." << std::endl;
        return false;
    }

    const auto& device = *device_it;
    const auto& users = identityService.getUsers();
    auto user_it = std::find_if(users.begin(), users.end(), [&](const User& u) {
        return u.username == device.assignedUser;
    });

    if (user_it == users.end()) {
        std::cerr << "Deployment failed: Assigned user not found." << std::endl;
        return false;
    }

    const auto& user = *user_it;

    std::cout << "Starting deployment for device: " << device.deviceId << std::endl;
    std::cout << "  User: " << user.username << std::endl;
    std::cout << "  Role: " << user.role << std::endl;
    std::cout << "  Generating role-specific Linux image..." << std::endl;
    std::cout << "  Configuring image with user credentials..." << std::endl;
    std::cout << "  Making image available for network boot..." << std::endl;
    std::cout << "  Deployment successful." << std::endl;

    return true;
}
