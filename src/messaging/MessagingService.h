#ifndef MESSAGING_SERVICE_H
#define MESSAGING_SERVICE_H

#include <string>
#include <zmq.hpp>

class MessagingService {
public:
    MessagingService();
    ~MessagingService();

    void sendMessage(const std::string& topic, const std::string& message);
    std::string receiveMessage(const std::string& topic);

private:
    zmq::context_t context;
    zmq::socket_t publisher;
    zmq::socket_t subscriber;
};

#endif // MESSAGING_SERVICE_H
