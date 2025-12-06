cmake_minimum_required(VERSION 3.10)

project(EnterpriseOS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Add ZeroMQ include directories and libraries
# find_package(ZeroMQ REQUIRED)
# include_directories(${ZeroMQ_INCLUDE_DIRS})
# link_libraries(${ZeroMQ_LIBRARIES})

add_executable(EnterpriseOS
    src/main.cpp
    src/identity/IdentityService.cpp
    src/device/DeviceService.cpp
    src/deployment/DeploymentService.cpp
    src/access/AccessService.cpp
    src/security/SecurityService.cpp
    src/messaging/MessagingService.cpp
    src/hr/HRService.cpp
    src/accounting/AccountingService.cpp
    src/ticketing/TicketingService.cpp
    src/file_management/FileManagementService.cpp
    src/monitoring/MonitoringService.cpp
)

target_link_libraries(EnterpriseOS pthread)
