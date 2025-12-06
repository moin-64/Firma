#include "MessagingService.h"
#include <optional>

MessagingService::MessagingService() : context(1), publisher(context, ZMQ_PUB), subscriber(context, ZMQ_SUB) {
    publisher.bind("tcp://*:5555");
    subscriber.connect("tcp://localhost:5555");
}

MessagingService::~MessagingService() {
}

void MessagingService::sendMessage(const std::string& topic, const std::string& message) {
    publisher.send(zmq::buffer(topic), zmq::send_flags::sndmore);
    publisher.send(zmq::buffer(message));
}

std::string MessagingService::receiveMessage(const std::string& topic) {
    subscriber.set(zmq::sockopt::subscribe, topic);

    zmq::message_t topic_msg;
    auto topic_result = subscriber.recv(topic_msg, zmq::recv_flags::none);

    zmq::message_t msg;
    auto msg_result = subscriber.recv(msg, zmq::recv_flags::none);

    if(!topic_result.has_value() || !msg_result.has_value()){
        return "";
    }

    return std::string(static_cast<char*>(msg.data()), msg.size());
}
