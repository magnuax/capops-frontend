#include <QPainter>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QUrl>
#include <QSize>

#include <cmath>

#include "services/TileMapServiceCARTO.hpp"
#include "domain/Coordinates.hpp"

TileMapServiceCARTO::TileMapServiceCARTO(QObject *parent) : ITileMapService(parent)
{
}

void TileMapServiceCARTO::enableDiskCache(const QString &dir, qint64 maxBytes)
{
    auto *cache = new QNetworkDiskCache(&_networkManager);
    cache->setCacheDirectory(dir);
    cache->setMaximumCacheSize(maxBytes);
    _networkManager.setCache(cache);
}

QPoint TileMapServiceCARTO::pixelToTile(const QPointF &pixel) const
{
    return {int(std::floor(pixel.x() / _tileSize.width())),
            int(std::floor(pixel.y() / _tileSize.height()))};
}

void TileMapServiceCARTO::fetch(const Request &request)
{
    _failed = false;
    _tileSize = request.tileSize;

    initProjection(
        request.minLat, request.maxLat,
        request.minLon, request.maxLon);

    initCanvas();
    fetchTiles();
}

void TileMapServiceCARTO::initProjection(double minLat, double maxLat, double minLon, double maxLon)
{
    _currentProjection = MapProjection(minLat, maxLat, minLon, maxLon);

    QPointF mercatorTopLeft = _currentProjection.mercatorBoundsTopLeft();
    QPointF mercatorBtmRight = _currentProjection.mercatorBoundsBtmRight();

    _topLeftTile = pixelToTile(mercatorTopLeft);
    _btmRightTile = pixelToTile(mercatorBtmRight);

    double canvasMinX = _topLeftTile.x() * _tileSize.width();
    double canvasMinY = _topLeftTile.y() * _tileSize.height();
    double canvasMaxX = (_btmRightTile.x() + 1) * _tileSize.width();
    double canvasMaxY = (_btmRightTile.y() + 1) * _tileSize.height();

    _canvasTopLeft = QPointF(canvasMinX, canvasMinY);
    _canvasBtmRight = QPointF(canvasMaxX, canvasMaxY);

    _cropRect = QRect(
        QPoint(
            int(std::round(mercatorTopLeft.x() - _canvasTopLeft.x())),
            int(std::round(mercatorTopLeft.y() - _canvasTopLeft.y()))),
        QPoint(
            int(std::round(mercatorBtmRight.x() - _canvasTopLeft.x())),
            int(std::round(mercatorBtmRight.y() - _canvasTopLeft.y()))));
}

void TileMapServiceCARTO::initCanvas()
{
    QSize canvasSize(
        int(std::ceil(_canvasBtmRight.x() - _canvasTopLeft.x())),
        int(std::ceil(_canvasBtmRight.y() - _canvasTopLeft.y())));

    _canvas = QImage(canvasSize, QImage::Format_RGB32);
    _canvas.fill(Qt::white);
    _pending = 0;
}

void TileMapServiceCARTO::fetchTiles()
{
    QNetworkRequest base;
    base.setRawHeader("User-Agent", "QtCapOps/0.1 (contact: magnua2005@gmail.com)");

    for (int tileX = _topLeftTile.x(); tileX <= _btmRightTile.x(); ++tileX)
    {
        for (int tileY = _topLeftTile.y(); tileY <= _btmRightTile.y(); ++tileY)
        {
            QNetworkRequest tileRequest(base);
            tileRequest.setUrl(_tileURL.arg(_currentProjection.zoom()).arg(tileX).arg(tileY));
            QNetworkReply *reply = _networkManager.get(tileRequest);
            _pending++;

            connect(reply, &QNetworkReply::finished, this, [=]()
                    { onTileFetched(reply, tileX, tileY); });
        }
    }
}

void TileMapServiceCARTO::onTileFetched(QNetworkReply *reply, int tileX, int tileY)
{
    if (!validateTileReply(reply))
    {
        return;
    }

    paintTile(reply, tileX, tileY);
    _pending--;

    if (_pending == 0 && !_failed)
    {
        finalizeTiles();
    }
}

bool TileMapServiceCARTO::validateTileReply(QNetworkReply *reply)
{
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status != 200)
    {
        qWarning() << "Tile HTTP" << status << reply->url();
    }

    if (_failed)
    {
        reply->deleteLater();
        return false;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        _failed = true;
        emit failed(QString("Tile failed: %1 (%2)")
                        .arg(reply->url().toString(), reply->errorString()));
        reply->deleteLater();
        return false;
    }

    return true;
}

void TileMapServiceCARTO::paintTile(QNetworkReply *reply, int tileX, int tileY)
{
    QImage tile;
    tile.loadFromData(reply->readAll());
    reply->deleteLater();

    if (tile.isNull())
    {
        _failed = true;
        emit failed("Downloaded tile was not a valid image.");
        return;
    }

    QPoint destination(
        int(std::round(tileX * _tileSize.width() - _canvasTopLeft.x())),
        int(std::round(tileY * _tileSize.height() - _canvasTopLeft.y())));

    QPainter painter(&_canvas);
    painter.drawImage(destination, tile);
}

void TileMapServiceCARTO::finalizeTiles()
{
    QImage cropped = _canvas.copy(_cropRect);

    QPointF croppedTopLeft = QPointF(_canvasTopLeft.x() + _cropRect.x(),
                                     _canvasTopLeft.y() + _cropRect.y());

    QPointF croppedBtmRight = QPointF(_canvasTopLeft.x() + _cropRect.right(),
                                      _canvasTopLeft.y() + _cropRect.bottom());

    _currentProjection.setCanvasOrigin(croppedTopLeft, croppedBtmRight);

    emit finished(QPixmap::fromImage(cropped), _currentProjection);
}
