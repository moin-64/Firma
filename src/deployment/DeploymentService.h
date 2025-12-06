#ifndef DEPLOYMENT_SERVICE_H
#define DEPLOYMENT_SERVICE_H

#include <string>

class DeploymentService {
public:
    DeploymentService();
    ~DeploymentService();

    bool deployImage(const std::string& deviceId, const std::string& imageId);
    bool updateImage(const std::string& deviceId, const std::string& imageId);

private:
    // Private members
};

#endif // DEPLOYMENT_SERVICE_H
