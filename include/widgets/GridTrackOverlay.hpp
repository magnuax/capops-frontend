#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QApplication>
#include <vector>

#include "domain/data/Track.hpp"
#include "domain/MapProjection.hpp"

class GridTrackOverlay : public QWidget
{
    Q_OBJECT

signals:
    void trackSelected(const QString &icao24);

public:
    explicit GridTrackOverlay(QWidget *parent = nullptr);

    void setTracks(const std::vector<Track> &tracks);
    void setProjection(const MapProjection &projection);
    void setSelectedTrack(const QString &icao24);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QString hitTest(const QPointF &widgetPos) const;
    QPointF trackWidgetPos(const Track &track) const;

    std::vector<Track> _tracks;
    double _minLat, _maxLat, _minLon, _maxLon;

    MapProjection _projection;
    QString _selectedIcao24;

    static constexpr int ICON_SIZE = 32;
    static constexpr int HIT_RADIUS = ICON_SIZE / 2 + 4;
};