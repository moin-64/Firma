#ifndef MESSAGING_SERVICE_H
#define MESSAGING_SERVICE_H

#include <string>

class MessagingService {
public:
    MessagingService();
    ~MessagingService();

    void sendMessage(const std::string& recipient, const std::string& message);
    std::string receiveMessage();

private:
    // Private members for ZeroMQ communication
};

#endif // MESSAGING_SERVICE_H
