#ifndef DEPLOYMENT_SERVICE_H
#define DEPLOYMENT_SERVICE_H

#include <string>
#include "../identity/IdentityService.h"
#include "../device/DeviceService.h"

class DeploymentService {
public:
    DeploymentService(const IdentityService& identityService, const DeviceService& deviceService);
    ~DeploymentService();

    bool deployImage(const std::string& deviceId);

private:
    const IdentityService& identityService;
    const DeviceService& deviceService;
};

#endif // DEPLOYMENT_SERVICE_H
