#include <QPainter>

#include "widgets/GridCanvas.hpp"

GridCanvas::GridCanvas(QWidget *parent)
    : QWidget(parent)
{
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

    QPixmap scaled = _mapSource.scaled(
        size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation);

    const int x = (scaled.width() - width()) / 2;
    const int y = (scaled.height() - height()) / 2;

    painter.drawPixmap(rect(), scaled, QRect(x, y, width(), height()));
}
