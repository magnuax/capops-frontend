#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <vector>

#include "domain/data/Track.hpp"
#include "domain/MapProjection.hpp"

class GridTrackOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit GridTrackOverlay(QWidget *parent = nullptr);

    void setTracks(const std::vector<Track> &tracks);

    void setProjection(const MapProjection &projection);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<Track> _tracks;
    double _minLat, _maxLat, _minLon, _maxLon;

    MapProjection _projection;
    QSize _pixmapSize;
};