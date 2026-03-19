
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
    _pixmapSize = projection.pixmapSize(); // see below
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
    if (_pixmapSize.isEmpty())
        return;

    double scale = std::min(
        (double)width() / _pixmapSize.width(),
        (double)height() / _pixmapSize.height());

    double offsetX = (width() - _pixmapSize.width() * scale) / 2.0;
    double offsetY = (height() - _pixmapSize.height() * scale) / 2.0;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Scale from pixmap coords to widget coords
    double scaleX = (double)width() / _pixmapSize.width();
    double scaleY = (double)height() / _pixmapSize.height();

    static QImage aircraftImage(":/icons/track.png");
    const int iconSize = 32;
    static QImage scaled = aircraftImage.scaled(
        iconSize,
        iconSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    for (const Track &track : _tracks)
    {
        QSize pixmapSize = _projection.pixmapSize();

        double scaleX = (double)width() / pixmapSize.width();
        double scaleY = (double)height() / pixmapSize.height();

        QPointF pixmapPos = _projection.toPixel(track.getPosition()[0], track.getPosition()[1]);

        QPointF widgetPos(
            pixmapPos.x() * scale + offsetX,
            pixmapPos.y() * scale + offsetY);

        QImage temp(iconSize * 2,
                    iconSize * 2,
                    QImage::Format_ARGB32_Premultiplied);

        temp.fill(Qt::transparent);

        QPainter tempPainter(&temp);
        tempPainter.setRenderHint(QPainter::Antialiasing);
        tempPainter.setRenderHint(QPainter::SmoothPixmapTransform);
        tempPainter.translate(iconSize, iconSize);
        tempPainter.rotate(track.getHeading());
        tempPainter.drawImage(-scaled.width() / 2, -scaled.height() / 2, scaled);
        tempPainter.end();

        painter.drawImage(widgetPos.x() - iconSize, widgetPos.y() - iconSize, temp);
    }
}