#include "httplib.h"
#include "json.hpp"

#include "identity/IdentityService.h"
#include "device/DeviceService.h"
#include "deployment/DeploymentService.h"
#include "access/AccessService.h"
#include "security/SecurityService.h"
#include "messaging/MessagingService.h"
#include "hr/HRService.h"
#include "accounting/AccountingService.h"
#include "ticketing/TicketingService.h"
#include "file_management/FileManagementService.h"
#include "monitoring/MonitoringService.h"

#include <iostream>

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    IdentityService identityService;
    DeviceService deviceService;
    DeploymentService deploymentService(identityService, deviceService);
    AccessService accessService;
    SecurityService securityService;
    MessagingService messagingService;
    HRService hrService;
    AccountingService accountingService;
    TicketingService ticketingService;
    FileManagementService fileManagementService;
    MonitoringService monitoringService;

    // Health check endpoint
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    // API endpoints
    svr.Post("/api/users", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        identityService.addUser(data["username"], data["password"]);
        identityService.assignRole(data["username"], data["role"]);
        res.set_content("User created", "text/plain");
    });

    svr.Get("/api/users", [&](const httplib::Request &, httplib::Response &res) {
        json users_json;
        for (const auto& [username, user] : identityService.getUsers()) {
            users_json[username] = {{"username", user.username}, {"role", user.role}};
        }
        res.set_content(users_json.dump(), "application/json");
    });

    svr.Post("/api/devices", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        deviceService.addDevice(data["deviceId"], data["deviceType"]);
        deviceService.assignDeviceToUser(data["deviceId"], data["assignedUser"]);
        res.set_content("Device created", "text/plain");
    });

    svr.Get("/api/devices", [&](const httplib::Request &, httplib::Response &res) {
        json devices_json;
        for (const auto& [deviceId, device] : deviceService.getDevices()) {
            devices_json[deviceId] = {{"deviceId", device.deviceId}, {"deviceType", device.deviceType}, {"assignedUser", device.assignedUser}};
        }
        res.set_content(devices_json.dump(), "application/json");
    });

    svr.Post("/api/deploy", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        deploymentService.deployImage(data["deviceId"]);
        res.set_content("Deployment started", "text/plain");
    });

    std::cout << "EnterpriseOS Core and Sub-System Services Initialized." << std::endl;
    std::cout << "Starting HTTP server on port 8080..." << std::endl;

    svr.listen("0.0.0.0", 8080);

    return 0;
}
