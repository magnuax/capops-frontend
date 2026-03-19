#pragma once
#include <QPointF>
#include <QSize>

#include <cmath>

class MapProjection
{
public:
    MapProjection() = default;
    MapProjection(double minLat, double maxLat, double minLon, double maxLon);

    void setCanvasOrigin(QPointF canvasTopLeft, QPointF canvasBtmRight);

    QPointF toPixel(double lat, double lon) const;
    QPointF toLonLat(double x, double y) const;

    QPointF mercatorBoundsTopLeft() const;
    QPointF mercatorBoundsBtmRight() const;

    int zoom() const;
    double minLat() const;
    double maxLat() const;
    double minLon() const;
    double maxLon() const;

    QSize pixmapSize() const;
    
private:
    double _minLat = 0, _maxLat = 0;
    double _minLon = 0, _maxLon = 0;
    int _zoom = 0;

    QPointF _canvasTopLeft;
    QPointF _canvasBtmRight;

    int computeZoom(int maxPixels = 4096) const;
    QPointF mercatorPixel(double lat, double lon) const;
    QPointF mercatorPixelAtZoom(double lat, double lon, int zoom) const;

};