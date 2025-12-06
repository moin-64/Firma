#include "DeploymentService.h"
#include <iostream>

DeploymentService::DeploymentService(const IdentityService& identityService, const DeviceService& deviceService)
    : identityService(identityService), deviceService(deviceService) {
}

DeploymentService::~DeploymentService() {
}

bool DeploymentService::deployImage(const std::string& deviceId) {
    const auto& devices = deviceService.getDevices();
    if (devices.find(deviceId) == devices.end()) {
        std::cerr << "Deployment failed: Device not found." << std::endl;
        return false;
    }

    const auto& device = devices.at(deviceId);
    const auto& users = identityService.getUsers();
    if (users.find(device.assignedUser) == users.end()) {
        std::cerr << "Deployment failed: Assigned user not found." << std::endl;
        return false;
    }

    const auto& user = users.at(device.assignedUser);

    std::cout << "Starting deployment for device: " << device.deviceId << std::endl;
    std::cout << "  User: " << user.username << std::endl;
    std::cout << "  Role: " << user.role << std::endl;
    std::cout << "  Generating role-specific Linux image..." << std::endl;
    std::cout << "  Configuring image with user credentials..." << std::endl;
    std::cout << "  Making image available for network boot..." << std::endl;
    std::cout << "  Deployment successful." << std::endl;

    return true;
}
