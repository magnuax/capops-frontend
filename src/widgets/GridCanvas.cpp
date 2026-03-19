#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

#include "widgets/GridCanvas.hpp"

GridCanvas::GridCanvas(QWidget *parent)
    : QWidget(parent)
{
}

void GridCanvas::setProjection(const MapProjection &projection)
{
    _projection = projection;
    update();
}

void GridCanvas::setMapSource(const QPixmap &pixmap)
{
    _mapSource = pixmap;
    update();
}

void GridCanvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (_mapSource.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPixmap scaled = _mapSource.scaled(
        size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    const int x = (scaled.width() - width()) / 2;
    const int y = (scaled.height() - height()) / 2;

    painter.drawPixmap(rect(), scaled, QRect(x, y, width(), height()));
}

void GridCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (_mapSource.isNull())
        return;

    double scale = std::min(
        (double)width() / _mapSource.width(),
        (double)height() / _mapSource.height());

    double offsetX = (width() - _mapSource.width() * scale) / 2.0;
    double offsetY = (height() - _mapSource.height() * scale) / 2.0;

    double pixmapX = (event->pos().x() - offsetX) / scale;
    double pixmapY = (event->pos().y() - offsetY) / scale;

    QPointF lonLat = _projection.toLonLat(pixmapX, pixmapY);
    emit cursorLatLon(lonLat.y(), lonLat.x());

    QWidget::mouseMoveEvent(event);
}

void GridCanvas::leaveEvent(QEvent *event)
{
    emit cursorLatLon(qQNaN(), qQNaN());
    QWidget::leaveEvent(event);
}