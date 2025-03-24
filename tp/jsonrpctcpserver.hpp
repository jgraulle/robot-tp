#ifndef JSONRPCCLIENT_HPP
#define JSONRPCCLIENT_HPP

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Document/DynamicJsonDocument.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>
#include <asio/io_context.hpp>
#include <functional>
#include <set>
#include <map>


class JsonRpcTcpServer
{
public:
    JsonRpcTcpServer(unsigned short tcpPort);
    virtual ~JsonRpcTcpServer();

    void listen();
    void session();

    void sendNotification(const std::string & methodName, const ArduinoJson::JsonObject & params);

    using Method = std::function<ArduinoJson::DynamicJsonDocument(ArduinoJson::JsonObject)>;
    void bindMethod(const std::string & methodName, const Method & method);

    using Notification = std::function<void(ArduinoJson::JsonObject)>;
    void bindNotification(const std::string & notificationName, const Notification & notification);

    using ParamsGenerator = std::function<ArduinoJson::DynamicJsonDocument()>;
    void bindOnConnectSendNotification(const std::string & methodName, const ParamsGenerator & paramsGenerator);

private:
    asio::io_context _ioc;
    unsigned short _tcpPort;
    std::map<std::string, Method> _methods;
    std::mutex _methodsMutex;
    std::map<std::string, Notification> _notifications;
    std::mutex _notificationsMutex;
    std::unique_ptr<asio::ip::tcp::socket> _clientSocket;
    std::mutex _clientSocketMutex;
    std::map<std::string, ParamsGenerator> _onConnectSendNotification;
    std::mutex _onConnectSendNotificationMutex;
};

#endif