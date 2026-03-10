#include <QPainter>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QUrl>
#include <cmath>

#include "services/TileMapServiceCARTO.hpp"
#include "domain/Coordinates.hpp"

TileMapServiceCARTO::TileMapServiceCARTO(QObject *parent) : ITileMapService(parent) {}

void TileMapServiceCARTO::enableDiskCache(const QString &dir, qint64 maxBytes)
{
    auto *cache = new QNetworkDiskCache(&_networkManager);
    cache->setCacheDirectory(dir);
    cache->setMaximumCacheSize(maxBytes);
    _networkManager.setCache(cache);
}

QPointF TileMapServiceCARTO::coordsToPixel(const Coordinates &coords, int zoom) const
{
    // Uses Web Mercator projection (EPSG:3857)

    const double zoomFactor = std::pow(2.0, zoom);
    const double scaleFactor = 128.0 / M_PI * zoomFactor;

    const double lon = coords.lon.rad();
    const double lat = coords.lat.rad();

    const double x = scaleFactor * (lon + M_PI);
    const double y = scaleFactor * (M_PI - std::log(std::tan(M_PI / 4.0 + lat / 2.0)));

    return {x, y};
}

QPoint TileMapServiceCARTO::pixelToTile(const QPointF &pixel) const
{
    return {int(std::floor(pixel.x() / _currentRequest.tileSize.width())),
            int(std::floor(pixel.y() / _currentRequest.tileSize.height()))};
}

void TileMapServiceCARTO::fetch(const Request &request)
{
    _currentRequest = request;
    _failed = false;

    _canvas = QImage(_currentRequest.imageSize, QImage::Format_Grayscale8);

    const QPointF centerPixel = coordsToPixel(
        _currentRequest.coords,
        _currentRequest.zoom);

    const QPointF diagonal = QPointF(
        _currentRequest.imageSize.width(),
        _currentRequest.imageSize.height());

    _canvasTopLeft = centerPixel - diagonal / 2.0;
    _topLeftTile = pixelToTile(_canvasTopLeft);

    _canvasBtmRight = centerPixel + diagonal / 2.0 - QPointF(1, 1);
    _btmRightTile = pixelToTile(_canvasBtmRight);

    _pending = 0;

    QNetworkRequest base;
    base.setRawHeader(
        "User-Agent",
        "QtCapOps/0.1 (contact: magnua2005@gmail.com)");

    for (int tileX = _topLeftTile.x(); tileX <= _btmRightTile.x(); ++tileX)
    {
        for (int tileY = _topLeftTile.y(); tileY <= _btmRightTile.y(); ++tileY)
        {

            QNetworkRequest request(base);
            request.setUrl(_tileURL.arg(_currentRequest.zoom).arg(tileX).arg(tileY));
            QNetworkReply *reply = _networkManager.get(request);
            _pending++;

            connect(reply, &QNetworkReply::finished, this, [=]()
                    { onTileFetched(reply, tileX, tileY); });
        }
    }
}

void TileMapServiceCARTO::onTileFetched(QNetworkReply *reply, int tileX, int tileY)
{

    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (status != 200)
    {
        qWarning() << "Tile HTTP" << status << reply->url();
    }

    if (_failed)
    {
        reply->deleteLater();
        return;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        _failed = true;

        const QString reply_str = reply->url().toString();
        const QString error_str = reply->errorString();
        const QString error = QString("Tile failed: %1 (%2)").arg(reply_str, error_str);

        reply->deleteLater();
        emit failed(error);
        return;
    }

    QImage tile;
    tile.loadFromData(reply->readAll());
    reply->deleteLater();

    if (tile.isNull())
    {
        _failed = true;
        emit failed("Downloaded tile was not a valid image.");
        return;
    }

    const double tileOriginX = tileX * _currentRequest.tileSize.width();
    const double tileOriginY = tileY * _currentRequest.tileSize.height();

    QPoint destination(
        int(std::round(tileOriginX - _canvasTopLeft.x())),
        int(std::round(tileOriginY - _canvasTopLeft.y())));

    QPainter painter(&_canvas);
    painter.drawImage(destination, tile);

    _pending--;
    if (_pending == 0 && !_failed)
    {
        emit finished(QPixmap::fromImage(_canvas));
    }
}