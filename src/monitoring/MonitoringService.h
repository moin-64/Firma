#ifndef MONITORING_SERVICE_H
#define MONITORING_SERVICE_H

#include <string>

class MonitoringService {
public:
    MonitoringService();
    ~MonitoringService();

    void checkSystemHealth();
    void monitorNetwork();

private:
    // Private members
};

#endif // MONITORING_SERVICE_H
