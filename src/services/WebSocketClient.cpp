#include <QUrl>
#include <QObject>
#include <QDebug>

#include "services/WebSocketClient.hpp"

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject(parent)
{
    connect(&_socket, &QWebSocket::connected, this, []()
            { qDebug() << "Connected to server"; });

    connect(&_socket, &QWebSocket::textMessageReceived, this, [](const QString &msg)
            { qDebug() << "Received:" << msg; });

    connect(&_socket, &QWebSocket::disconnected, this, []()
            { qDebug() << "Disconnected"; });

    connect(&_socket, &QWebSocket::errorOccurred, this, [](QAbstractSocket::SocketError)
            { qDebug() << "WebSocket error"; });
}

void WebSocketClient::connectToServer()
{
    _socket.open(QUrl("ws://127.0.0.1:8080"));
}

void WebSocketClient::sendMessage(const QString &message)
{
    _socket.sendTextMessage(message);
}