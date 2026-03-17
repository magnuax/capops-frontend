#include <QUrl>
#include <QObject>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "services/WebSocketClient.hpp"

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject(parent)
{
    connect(&_socket, &QWebSocket::connected, this, &WebSocketClient::onConnected);

    connect(&_socket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onMessageReceived);

    connect(&_socket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);

    connect(&_socket, &QWebSocket::errorOccurred, this, &WebSocketClient::onError);
}

void WebSocketClient::connectToServer(const QUrl &url)
{
    _socket.open(url);
}

void WebSocketClient::parseMessage(const QJsonObject &json)
{
    const QString type = json["type"].toString();

    if (type == "risk")
        emit sectorRiskUpdated(json["sectorId"].toInt(),
                               riskStateFromString(json["value"].toString()));
    else if (type == "weather")
        emit sectorWeatherUpdated(json["sectorId"].toInt(),
                                  weatherStateFromString(json["value"].toString()));
    else if (type == "traffic")
        emit sectorTrafficUpdated(json["sectorId"].toInt(),
                                  trafficStateFromString(json["value"].toString()));
    else if (type == "flights")
    {
        std::vector<std::string> ids;
        for (const auto &v : json["flights"].toArray())
            ids.push_back(v.toString().toStdString());
        emit sectorFlightsUpdated(json["sectorId"].toInt(), ids);
    }
}

void WebSocketClient::onConnected()
{
    emit connected();
}

void WebSocketClient::onDisconnected()
{
    emit disconnected();
}

void WebSocketClient::onError(QAbstractSocket::SocketError error)
{
    emit errorOccurred(_socket.errorString());
}

void WebSocketClient::onMessageReceived(const QString &message)
{
    const QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();
    parseMessage(json);
}