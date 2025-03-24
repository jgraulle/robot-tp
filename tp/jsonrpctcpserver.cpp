#include "jsonrpctcpserver.hpp"

#include <ArduinoJson.hpp>
#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <utility>
#include <memory>
#include <iostream>
#include <asio/streambuf.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>
#include <asio/read.hpp>
#include <asio/buffer.hpp>
#include <thread>

const int max_length = 1024;

JsonRpcTcpServer::JsonRpcTcpServer(unsigned short tcpPort)
    : _ioc()
    , _tcpPort(tcpPort)
{
}

JsonRpcTcpServer::~JsonRpcTcpServer()
{
}

void JsonRpcTcpServer::sendNotification(const std::string & methodName, const ArduinoJson::JsonObject & params)
{
    // Prepare message
    ArduinoJson::StaticJsonDocument<256> message;
    message["jsonrpc"] = "2.0";
    message["method"] = methodName;
    message["params"] = params;

    asio::streambuf tcpStreambuf;
    std::ostream tcpOutStream(&tcpStreambuf);
    {
        const std::lock_guard<std::mutex> lock(_clientSocketMutex);
        if (_clientSocket.get() != nullptr)
        {
            ArduinoJson::serializeJson(message, tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            asio::write(*_clientSocket, tcpStreambuf);
            assert(tcpStreambuf.size() == 0);
        }
    }

#ifdef JSONRPC_MSG_DEBUG
    // Print notification
    std::cout << "Send notification to all client ";
    ArduinoJson::serializeJson(message, std::cout);
    std::cout << std::endl;
#endif
}

void JsonRpcTcpServer::bindMethod(const std::string & methodName, const Method & method)
{
    const std::lock_guard<std::mutex> lock(_methodsMutex);
    _methods.insert(std::make_pair(methodName, method));
}

void JsonRpcTcpServer::bindNotification(const std::string & notificationName, const Notification & notification)
{
    const std::lock_guard<std::mutex> lock(_notificationsMutex);
    _notifications.insert(std::make_pair(notificationName, notification));
}

void JsonRpcTcpServer::bindOnConnectSendNotification(const std::string & methodName, const ParamsGenerator & paramsGenerator)
{
    const std::lock_guard<std::mutex> lock(_onConnectSendNotificationMutex);
    _onConnectSendNotification.insert(std::make_pair(methodName, paramsGenerator));
}

void JsonRpcTcpServer::listen()
{
    std::cout << "Listen on TCP port " << _tcpPort << std::endl;
    asio::ip::tcp::acceptor acceptor(_ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _tcpPort));
    while (true)
    {
        auto clientSocket = std::make_unique<asio::ip::tcp::socket>(_ioc);
        acceptor.accept(*clientSocket);
        {
            const std::lock_guard<std::mutex> lock(_clientSocketMutex);
            _clientSocket = std::move(clientSocket);
        }
#ifdef JSONRPC_CONNECT_DEBUG
        std::cout << "New client connected" << std::endl;
#endif
        session();
    }
}

void JsonRpcTcpServer::session()
{
    asio::streambuf tcpInStreambuf;
    std::istream tcpInStream(&tcpInStreambuf);
    asio::streambuf tcpOutStreambuf;
    std::ostream tcpOutStream(&tcpOutStreambuf);
    ArduinoJson::StaticJsonDocument<256> messageReceived;
    ArduinoJson::StaticJsonDocument<256> messageToSend;

    // Send on connect notifications
    {
        const std::lock_guard<std::mutex> lock(_onConnectSendNotificationMutex);
        for (auto & sendNotification : _onConnectSendNotification)
        {
            // Prepare message
            messageToSend["jsonrpc"] = "2.0";
            messageToSend["method"] = sendNotification.first;
            messageToSend["params"] = sendNotification.second().as<ArduinoJson::JsonObject>();
            // Send message
            ArduinoJson::serializeJson(messageToSend, tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            {
                const std::lock_guard<std::mutex> lockSockets(_clientSocketMutex);
                asio::write(*_clientSocket, tcpOutStreambuf);
            }
            assert(tcpOutStreambuf.size() == 0);
            messageToSend.clear();
        }
    }

    while (_clientSocket->is_open())
    {
        try {

        // Wait receive complete message
#ifdef JSONRPC_MSG_DEBUG
        std::cout << "wait message..." << std::endl;
#endif
        try {
            asio::read_until(*_clientSocket, tcpInStreambuf, 0x0A);
        }
        catch (asio::system_error & e)
        {
            if (e.code().value() == asio::error::eof)
                break;
            throw e;
        }

        // Extract one json message
        std::string messageReceivedStr;
        std::getline(tcpInStream, messageReceivedStr, static_cast<char>(0x0A));

        // Parse this json message
        ArduinoJson::DeserializationError error = ArduinoJson::deserializeJson(messageReceived, messageReceivedStr);
        if (error.code() != ArduinoJson::DeserializationError::Ok)
            throw std::runtime_error(error.c_str());
        messageReceivedStr.clear();

#ifdef JSONRPC_MSG_DEBUG
        // Print receive message
        std::cout << "Receive ";
        ArduinoJson::serializeJson(message, std::cout);
        std::cout << std::endl;
#endif

        std::string methodName = messageReceived["method"].as<const char*>();
        ArduinoJson::JsonObject params = messageReceived["params"].as<ArduinoJson::JsonObject>();

        // If notification
        if (!messageReceived.containsKey("id"))
        {
            // Find it and execute it
            const std::lock_guard<std::mutex> lock(_notificationsMutex);
            Notification * notification = nullptr;
            try {
                notification = &_notifications.at(methodName);
            }
            catch (std::out_of_range & e)
            {
                if (!_methods.empty() || !_notifications.empty())
                    std::cerr << "method \"" << methodName << "\" not supported!" << std::endl;
                break;
            }
            (*notification)(params);
            notification = nullptr;
        }
        // If method
        else
        {
            // Find it and execute it
            messageToSend["jsonrpc"] = "2.0";
            messageToSend["id"] = messageReceived["id"];
            {
                const std::lock_guard<std::mutex> lock(_methodsMutex);
                Method * method = nullptr;
                try {
                    method = &_methods.at(methodName);
                }
                catch (std::out_of_range & e)
                {
                    if (!_methods.empty() || !_notifications.empty())
                        std::cerr << "method \"" << methodName << "\" not supported!" << std::endl;
                    break;
                }
                messageToSend["result"] = (*method)(params).as<ArduinoJson::JsonObject>();
                method = nullptr;
            }

#ifdef JSONRPC_MSG_DEBUG
            // Print response
            std::cout << "Send response ";
            ArduinoJson::serializeJson(messageToSend, std::cout);
            std::cout << std::endl;
#endif

            // Send response
            ArduinoJson::serializeJson(messageToSend, tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            {
                const std::lock_guard<std::mutex> lock(_clientSocketMutex);
                asio::write(*_clientSocket, tcpOutStreambuf);
            }
            assert(tcpOutStreambuf.size() == 0);
            messageToSend.clear();
        }
        messageReceived.clear();

        } catch (std::exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

#ifdef JSONRPC_CONNECT_DEBUG
    std::cout << "Client disconnected" << std::endl;
#endif

    {
        const std::lock_guard<std::mutex> lock(_clientSocketMutex);
        _clientSocket->close();
        _clientSocket.release();
    }
}
