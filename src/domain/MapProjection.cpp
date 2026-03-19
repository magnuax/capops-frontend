#include "domain/MapProjection.hpp"

MapProjection::MapProjection(double minLat, double maxLat, double minLon, double maxLon)
    : _minLat(minLat), _maxLat(maxLat), _minLon(minLon), _maxLon(maxLon)
{
}

void MapProjection::setProjectionParams(int zoom, QPointF canvasTopLeft, QPointF canvasBtmRight)
{
    _zoom = zoom;
    _canvasTopLeft = canvasTopLeft;
    _canvasBtmRight = canvasBtmRight;
}

QPointF MapProjection::worldPixel(double lat, double lon) const
{
    const double zoomFactor = std::pow(2.0, _zoom);
    const double scaleFactor = 128.0 / M_PI * zoomFactor;

    const double lonRad = lon * M_PI / 180.0;
    const double latRad = lat * M_PI / 180.0;

    const double x = scaleFactor * (lonRad + M_PI);
    const double y = scaleFactor * (M_PI - std::log(std::tan(M_PI / 4.0 + latRad / 2.0)));

    return QPointF(x, y);
}

QPointF MapProjection::toPixel(double lat, double lon) const
{
    QPointF world = worldPixel(lat, lon);
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

QPointF MapProjection::canvasTopLeft() const
{
    return _canvasTopLeft;
}

QPointF MapProjection::canvasBtmRight() const
{
    return _canvasBtmRight;
}

QSize MapProjection::pixmapSize() const
{
    return QSize(
        int(std::ceil(_canvasBtmRight.x() - _canvasTopLeft.x())),
        int(std::ceil(_canvasBtmRight.y() - _canvasTopLeft.y())));
}

int MapProjection::zoom() const
{
    return _zoom;
}

double MapProjection::minLat()
{
    return _minLat;
}

double MapProjection::maxLat()
{
    return _maxLat;
}

double MapProjection::minLon()
{
    return _minLon;
}

double MapProjection::maxLon()
{
    return _maxLon;
}