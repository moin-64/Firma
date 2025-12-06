#include "httplib.h"
#include "json.hpp"

#include "core/Logger.h"
#include "core/ConfigService.h"
#include "core/DatabaseService.h"
#include "core/PKIService.h"
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
#include <thread>
#include <chrono>
#include <csignal>

using json = nlohmann::json;

std::unique_ptr<httplib::Server> svr;

void signal_handler(int signum) {
    Logger::log(LogLevel::INFO, "Interrupt signal (" + std::to_string(signum) + ") received.");
    if(svr) svr->stop();
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    ConfigService config("config.json");
    Logger::init(config.get("log_file"));
    Logger::log(LogLevel::INFO, "Starting EnterpriseOS...");

    DatabaseService dbService(config.get("db_conninfo"));

    svr = std::make_unique<httplib::Server>();

    PKIService pkiService;
    IdentityService identityService(&dbService);
    DeviceService deviceService(&dbService);
    DeploymentService deploymentService(identityService, deviceService);
    AccessService accessService;
    SecurityService securityService;
    MessagingService messagingService;
    HRService hrService;
    AccountingService accountingService;
    TicketingService ticketingService;
    FileManagementService fileManagementService;
    MonitoringService monitoringService;
    Logger::log(LogLevel::INFO, "All services initialized.");

    // Health check endpoint
    svr->Get("/health", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    // API endpoints
    svr->Post("/api/users", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        identityService.addUser(data["username"], data["password"]);
        identityService.assignRole(data["username"], data["role"]);
        pkiService.generateCertificate(data["username"]);
        Logger::log(LogLevel::INFO, "User created: " + data["username"].get<std::string>());
        messagingService.sendMessage("security.events", "{\"event_type\": \"user_created\", \"username\": \"" + data["username"].get<std::string>() + "\"}");
        res.set_content("User created", "text/plain");
    });

    svr->Get("/api/users", [&](const httplib::Request &, httplib::Response &res) {
        json users_json;
        for (const auto& user : identityService.getUsers()) {
            users_json[user.username] = {{"username", user.username}, {"role", user.role}};
        }
        res.set_content(users_json.dump(), "application/json");
    });

    svr->Post("/api/devices", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        deviceService.addDevice(data["deviceId"], data["deviceType"]);
        deviceService.assignDeviceToUser(data["deviceId"], data["assignedUser"]);
        Logger::log(LogLevel::INFO, "Device created: " + data["deviceId"].get<std::string>());
        messagingService.sendMessage("device.status", "{\"device_id\": \"" + data["deviceId"].get<std::string>() + "\", \"status\": \"registered\"}");
        res.set_content("Device created", "text/plain");
    });

    svr->Get("/api/devices", [&](const httplib::Request &, httplib::Response &res) {
        json devices_json;
        for (const auto& device : deviceService.getDevices()) {
            devices_json[device.deviceId] = {{"deviceId", device.deviceId}, {"deviceType", device.deviceType}, {"assignedUser", device.assignedUser}};
        }
        res.set_content(devices_json.dump(), "application/json");
    });

    svr->Post("/api/deploy", [&](const httplib::Request &req, httplib::Response &res) {
        auto data = json::parse(req.body);
        // This is a simplified implementation. A real implementation would use the device ID to look up the assigned user.
        // deploymentService.deployImage(data["deviceId"]);
        Logger::log(LogLevel::INFO, "Deployment started for device: " + data["deviceId"].get<std::string>());
        messagingService.sendMessage("deployment.jobs", "{\"action\": \"new\", \"device_id\": \"" + data["deviceId"].get<std::string>() + "\"}");
        res.set_content("Deployment started", "text/plain");
    });

    int port = config.get("server_port");
    Logger::log(LogLevel::INFO, "Starting HTTP server on port " + std::to_string(port));
    svr->listen("0.0.0.0", port);

    Logger::log(LogLevel::INFO, "EnterpriseOS shutting down.");

    return 0;
}
