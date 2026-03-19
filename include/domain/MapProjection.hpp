#pragma once
#include <QPointF>
#include <QSize>

#include <cmath>

class MapProjection
{
public:
    MapProjection() = default;
    MapProjection(double minLat, double maxLat, double minLon, double maxLon);

    // Call this once the tile service has determined the zoom and canvas origin
    void setProjectionParams(int zoom, QPointF canvasTopLeft, QPointF canvasBtmRight);

    QPointF toPixel(double lat, double lon) const;
    QPointF toLonLat(double x, double y) const;

    QPointF canvasTopLeft() const;
    QPointF canvasBtmRight() const;
    QSize pixmapSize() const;
    
    int zoom() const;
    double minLat();
    double maxLat();
    double minLon();
    double maxLon();

private:
    double _minLat = 0, _maxLat = 0;
    double _minLon = 0, _maxLon = 0;
    int _zoom = 0;

    QPointF _canvasTopLeft;
    QPointF _canvasBtmRight;

    QPointF worldPixel(double lat, double lon) const;
};