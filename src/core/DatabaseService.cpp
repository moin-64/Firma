#include "DatabaseService.h"
#include "Logger.h"

DatabaseService::DatabaseService(const std::string& conninfo) {
    conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        Logger::log(LogLevel::ERROR, "Connection to database failed: " + std::string(PQerrorMessage(conn)));
        PQfinish(conn);
        conn = nullptr;
    } else {
        Logger::log(LogLevel::INFO, "Connected to database successfully.");
    }
}

DatabaseService::~DatabaseService() {
    if (conn) {
        PQfinish(conn);
    }
}

PGconn* DatabaseService::getConnection() const {
    return conn;
}
