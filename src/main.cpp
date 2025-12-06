#include "httplib.h"

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

int main() {
    httplib::Server svr;

    IdentityService identityService;
    DeviceService deviceService;
    DeploymentService deploymentService;
    AccessService accessService;
    SecurityService securityService;
    MessagingService messagingService;
    HRService hrService;
    AccountingService accountingService;
    TicketingService ticketingService;
    FileManagementService fileManagementService;
    MonitoringService monitoringService;

    svr.Get("/health", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    std::cout << "EnterpriseOS Core and Sub-System Services Initialized." << std::endl;
    std::cout << "Starting HTTP server on port 8080..." << std::endl;

    svr.listen("0.0.0.0", 8080);

    return 0;
}
