#pragma once
#include <QWidget>

#include "domain/MapProjection.hpp"

class QPixmap;
class QPaintEvent;

class GridCanvas : public QWidget
{
    Q_OBJECT

signals:
    void cursorLatLon(double lat, double lon);

public:
    explicit GridCanvas(QWidget *parent = nullptr);

    void setProjection(const MapProjection &projection);

    void setMapSource(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPixmap _mapSource;

    double _minLat = -90.0;
    double _maxLat = 90.0;
    double _minLon = -180.0;
    double _maxLon = 180.0;
    MapProjection _projection;
};