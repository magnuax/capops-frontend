#pragma once

#include <QObject>
#include <QPixmap>
#include <QSize>

#include "domain/Coordinates.hpp"

class ITileMapService : public QObject
{
    Q_OBJECT

public:
    explicit ITileMapService(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~ITileMapService() = default;

    struct Request
    {
        int zoom = 13;
        Coordinates coords = {0.0, 0.0};
        QSize imageSize = {512, 512};
        QSize tileSize = {256, 256};
    };

    virtual void fetch(const Request &request) = 0;

    virtual void enableDiskCache(const QString &cacheDir, qint64 maxBytes = 256LL * 1024 * 1024) = 0;

signals:
    void finished(const QPixmap &map);
    void failed(const QString &error);
};