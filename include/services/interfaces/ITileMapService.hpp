#pragma once

#include <QObject>
#include <QPixmap>
#include <QSize>

#include "domain/Coordinates.hpp"
#include "domain/MapProjection.hpp"

class ITileMapService : public QObject
{
    Q_OBJECT

signals:
    void finished(const QPixmap &map, const MapProjection &projection);
    void failed(const QString &error);

public:
    explicit ITileMapService(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~ITileMapService() = default;

    struct Request
    {
        double minLat = 0, maxLat = 0;
        double minLon = 0, maxLon = 0;

        QSize tileSize = {256, 256};
    };

    virtual void fetch(const Request &request) = 0;

    virtual void enableDiskCache(const QString &cacheDir, qint64 maxBytes = 256LL * 1024 * 1024) = 0;
};