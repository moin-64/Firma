#include "PKIService.h"
#include "Logger.h"

PKIService::PKIService() {
}

PKIService::~PKIService() {
}

std::string PKIService::generateCertificate(const std::string& userId) {
    Logger::log(LogLevel::INFO, "Generating dummy certificate for user: " + userId);
    return "dummy_certificate_for_" + userId;
}

bool PKIService::signCertificate(const std::string& certificate) {
    Logger::log(LogLevel::INFO, "Signing dummy certificate: " + certificate);
    return true;
}
