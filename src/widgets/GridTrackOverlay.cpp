
#include <QPainter>

#include "widgets/GridTrackOverlay.hpp"
#include "domain/MapProjection.hpp"

GridTrackOverlay::GridTrackOverlay(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setStyleSheet("background: transparent;");

    _minLat = -90.0;
    _maxLat = 90.0;
    _minLon = -180.0;
    _maxLon = 180.0;
}

void GridTrackOverlay::setProjection(const MapProjection &projection)
{
    _projection = projection;
    update();
}

void GridTrackOverlay::setTracks(const std::vector<Track> &tracks)
{
    if (tracks == _tracks)
        return;

    _tracks = tracks;

    update();
}

void GridTrackOverlay::paintEvent(QPaintEvent *event)
{
    QSize pixmapSize = _projection.pixmapSize();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    static QImage trackIcon(":/icons/track.png");
    const int iconSize = 32;
    static QImage scaledTrackIcon = trackIcon.scaled(
        iconSize,
        iconSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    double scale = std::min(
        (double)width() / pixmapSize.width(),
        (double)height() / pixmapSize.height());

    double offsetX = (width() - pixmapSize.width() * scale) / 2.0;
    double offsetY = (height() - pixmapSize.height() * scale) / 2.0;

    for (const Track &track : _tracks)
    {
        QSize pixmapSize = _projection.pixmapSize();

        QPointF pixmapPos = _projection.toPixel(track.getPosition()[0], track.getPosition()[1]);

        QPointF widgetPos(
            pixmapPos.x() * scale + offsetX,
            pixmapPos.y() * scale + offsetY);

        painter.save();
        painter.translate(widgetPos);
        painter.rotate(track.getHeading());
        painter.drawImage(
            -scaledTrackIcon.width() / 2,
            -scaledTrackIcon.height() / 2,
            scaledTrackIcon);
        painter.restore();
    }
}