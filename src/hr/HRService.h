#ifndef HR_SERVICE_H
#define HR_SERVICE_H

#include <string>

class HRService {
public:
    HRService();
    ~HRService();

    bool addEmployee(const std::string& employeeId, const std::string& name);
    bool removeEmployee(const std::string& employeeId);
    bool manageLeave(const std::string& employeeId, const std::string& leaveType);

private:
    // Database connection and other private members
};

#endif // HR_SERVICE_H
