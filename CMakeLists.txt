cmake_minimum_required(VERSION 3.10)

project(EnterpriseOS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include src)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
pkg_check_modules(PQ REQUIRED libpq)

include_directories(${ZMQ_INCLUDE_DIRS} ${PQ_INCLUDE_DIRS})

add_executable(EnterpriseOS
    src/main.cpp
    src/core/Logger.cpp
    src/core/ConfigService.cpp
    src/core/DatabaseService.cpp
    src/core/PKIService.cpp
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

target_link_libraries(EnterpriseOS pthread ${ZMQ_LIBRARIES} ${PQ_LIBRARIES})
