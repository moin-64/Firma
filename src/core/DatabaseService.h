#ifndef DATABASE_SERVICE_H
#define DATABASE_SERVICE_H

#include <libpq-fe.h>
#include <string>

class DatabaseService {
public:
    DatabaseService(const std::string& conninfo);
    ~DatabaseService();

    PGconn* getConnection() const;

private:
    PGconn* conn;
};

#endif // DATABASE_SERVICE_H
