#include "domain/MapProjection.hpp"

MapProjection::MapProjection(double minLat, double maxLat, double minLon, double maxLon)
    : _minLat(minLat), _maxLat(maxLat), _minLon(minLon), _maxLon(maxLon)
{
    _zoom = computeZoom();
}

void MapProjection::setCanvasOrigin(QPointF canvasTopLeft, QPointF canvasBtmRight)
{
    _canvasTopLeft = canvasTopLeft;
    _canvasBtmRight = canvasBtmRight;
}

QPointF MapProjection::mercatorBoundsTopLeft() const
{
    return mercatorPixel(_maxLat, _minLon);
}

QPointF MapProjection::mercatorBoundsBtmRight() const
{
    return mercatorPixel(_minLat, _maxLon);
}

QPointF MapProjection::toPixel(double lat, double lon) const
{
    QPointF world = mercatorPixel(lat, lon);
    return world - _canvasTopLeft;
}

QPointF MapProjection::toLonLat(double x, double y) const
{
    const double zoomFactor = std::pow(2.0, _zoom);
    const double scaleFactor = 128.0 / M_PI * zoomFactor;

    const double lonRad = (x + _canvasTopLeft.x()) / scaleFactor - M_PI;
    const double latRad = 2.0 * std::atan(std::exp(M_PI - (y + _canvasTopLeft.y()) / scaleFactor)) - M_PI / 2.0;

    return QPointF(lonRad * 180.0 / M_PI, latRad * 180.0 / M_PI);
}

int MapProjection::zoom() const
{
    return _zoom;
}

double MapProjection::minLat() const
{
    return _minLat;
}

double MapProjection::maxLat() const
{
    return _maxLat;
}

double MapProjection::minLon() const
{
    return _minLon;
}

double MapProjection::maxLon() const
{
    return _maxLon;
}

int MapProjection::computeZoom(int maxPixels) const
{
    for (int zoom = 10; zoom >= 0; --zoom)
    {
        QPointF topLeft = mercatorPixelAtZoom(_maxLat, _minLon, zoom);
        QPointF btmRight = mercatorPixelAtZoom(_minLat, _maxLon, zoom);

        if (btmRight.x() - topLeft.x() <= maxPixels &&
            btmRight.y() - topLeft.y() <= maxPixels)
            return zoom;
    }
    return 0;
}

QPointF MapProjection::mercatorPixelAtZoom(double lat, double lon, int zoom) const
{
    const double zoomFactor = std::pow(2.0, zoom);
    const double scaleFactor = 128.0 / M_PI * zoomFactor;

    const double lonRad = lon * M_PI / 180.0;
    const double latRad = lat * M_PI / 180.0;

    const double x = scaleFactor * (lonRad + M_PI);
    const double y = scaleFactor * (M_PI - std::log(std::tan(M_PI / 4.0 + latRad / 2.0)));

    return QPointF(x, y);
}

QPointF MapProjection::mercatorPixel(double lat, double lon) const
{
    return mercatorPixelAtZoom(lat, lon, _zoom);
}

QSize MapProjection::pixmapSize() const
{
    return QSize(
        int(std::ceil(_canvasBtmRight.x() - _canvasTopLeft.x())),
        int(std::ceil(_canvasBtmRight.y() - _canvasTopLeft.y())));
}