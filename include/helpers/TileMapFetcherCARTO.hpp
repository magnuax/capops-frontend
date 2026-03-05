#pragma once

#include <QObject>
#include <QPixmap>
#include <QSize>
#include <QNetworkAccessManager>

#include "helpers/MapUtils.hpp"

class TileMapFetcherCARTO : public QObject
{
    Q_OBJECT

public:
    explicit TileMapFetcherCARTO(QObject *parent = nullptr);

    struct Request
    {
        int zoom = 13;
        Coordinates coords = {0.0, 0.0};
        QSize imageSize = {512, 512};
        QSize tileSize = {256, 256};
    };

    void fetch(const Request &request);

    void enableDiskCache(const QString &cacheDir, qint64 maxBytes = 256LL * 1024 * 1024);

signals:
    void finished(const QPixmap &map);
    void failed(const QString &error);

private slots:
    void onTileFetched(QNetworkReply *reply, int tileX, int tileY);

private:
    QPointF coordsToPixel(const Coordinates &coords, int zoom) const;
    QPoint pixelToTile(const QPointF &pixel) const;

    QNetworkAccessManager _networkManager;
    Request _currentRequest;

    QImage _canvas;
    QPointF _canvasTopLeft;
    QPointF _canvasBtmRight;
    QPoint _topLeftTile;
    QPoint _btmRightTile;

    QString _tileURL = "https://basemaps.cartocdn.com/light_nolabels/%1/%2/%3.png";

    int _pending = 0;
    bool _failed = false;
};