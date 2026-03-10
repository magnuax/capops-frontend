#pragma once
#include <QNetworkAccessManager>

#include "helpers/ITileMapService.hpp"
#include "domain/Coordinates.hpp"

class QNetworkReply;
class QImage;

class TileMapServiceCARTO : public ITileMapService
{
    Q_OBJECT

public:
    explicit TileMapServiceCARTO(QObject *parent = nullptr);
    ~TileMapServiceCARTO() override = default;

    void fetch(const Request &request) override;

    void enableDiskCache(const QString &cacheDir, qint64 maxBytes = 256LL * 1024 * 1024) override;

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