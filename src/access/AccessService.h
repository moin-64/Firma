#ifndef ACCESS_SERVICE_H
#define ACCESS_SERVICE_H

#include <string>

class AccessService {
public:
    AccessService();
    ~AccessService();

    bool grantAccess(const std::string& userId, const std::string& terminalId);
    bool revokeAccess(const std::string& userId, const std::string& terminalId);

private:
    // Private members
};

#endif // ACCESS_SERVICE_H
