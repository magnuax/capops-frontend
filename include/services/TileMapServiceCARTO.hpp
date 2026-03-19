#pragma once
#include <QNetworkAccessManager>

#include "services/interfaces/ITileMapService.hpp"
#include "domain/Coordinates.hpp"

class QNetworkReply;
class QImage;
class QSize;

class TileMapServiceCARTO : public ITileMapService
{
    Q_OBJECT

public:
    explicit TileMapServiceCARTO(QObject *parent = nullptr);

    ~TileMapServiceCARTO() override = default;

    void enableDiskCache(const QString &cacheDir, qint64 maxBytes = 256LL * 1024 * 1024) override;

    void fetch(const Request &request) override;
    void initProjection(double minLat, double maxLat, double minLon, double maxLon);
    void initCanvas();
    void fetchTiles();

    bool validateTileReply(QNetworkReply *reply);
    void paintTile(QNetworkReply *reply, int tileX, int tileY);
    void finalizeTiles();

private slots:
    void onTileFetched(QNetworkReply *reply, int tileX, int tileY);

private:
    QPoint pixelToTile(const QPointF &pixel) const;

    QNetworkAccessManager _networkManager;

    MapProjection _currentProjection;
    QRect _cropRect;

    QImage _canvas;
    QPointF _canvasTopLeft;
    QPointF _canvasBtmRight;

    QSize _tileSize;
    QPoint _topLeftTile;
    QPoint _btmRightTile;

    QString _tileURL = "https://basemaps.cartocdn.com/light_nolabels/%1/%2/%3.png";
    int _pending = 0;
    bool _failed = false;
};