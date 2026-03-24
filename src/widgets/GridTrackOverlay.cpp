
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <cmath>

#include "widgets/GridTrackOverlay.hpp"
#include "domain/MapProjection.hpp"

GridTrackOverlay::GridTrackOverlay(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_NoSystemBackground, true);
    setStyleSheet("background: transparent;");
    setMouseTracking(true);

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

    raise();
    update();
}

void GridTrackOverlay::setSelectedTrack(const QString &icao24)
{
    if (_selectedIcao24 == icao24)
        return;

    _selectedIcao24 = icao24;
    update();
}

QPointF GridTrackOverlay::trackWidgetPos(const Track &track) const
{
    QSize pixmapSize = _projection.pixmapSize();
    if (pixmapSize.isEmpty())
        return {};

    double scale = std::min(
        (double)width() / pixmapSize.width(),
        (double)height() / pixmapSize.height());

    double offsetX = (width() - pixmapSize.width() * scale) / 2.0;
    double offsetY = (height() - pixmapSize.height() * scale) / 2.0;

    QPointF pixmapPos = _projection.toPixel(track.getPosition()[0], track.getPosition()[1]);

    return QPointF(
        pixmapPos.x() * scale + offsetX,
        pixmapPos.y() * scale + offsetY);
}

QString GridTrackOverlay::hitTest(const QPointF &widgetPos) const
{
    for (const Track &track : _tracks)
    {
        QPointF pos = trackWidgetPos(track);
        double dx = widgetPos.x() - pos.x();
        double dy = widgetPos.y() - pos.y();

        if (dx * dx + dy * dy <= HIT_RADIUS * HIT_RADIUS)
            return track.getIcao24();
    }
    return {};
}

void GridTrackOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        event->ignore();
        return;
    }

    QString icao24 = hitTest(event->pos());

    if (!icao24.isEmpty())
    {
        _selectedIcao24 = icao24;
        emit trackSelected(icao24);
        update();
        event->accept();
        return;
    }

    // No track hit — forward click to the widget underneath this overlay.
    QWidget *parent = parentWidget();
    if (parent)
    {
        QPoint parentPos = mapToParent(event->pos());

        QWidget *child = parent->childAt(parentPos);
        if (child == this)
        {
            // Temporarily disable hit-testing on the overlay so childAt can
            // resolve the real target widget below it.
            setAttribute(Qt::WA_TransparentForMouseEvents, true);
            child = parent->childAt(parentPos);
            setAttribute(Qt::WA_TransparentForMouseEvents, false);
        }

        if (child && child != this)
        {
            QPointF childPos = child->mapFrom(parent, parentPos);
            QMouseEvent forwarded(
                event->type(), childPos, event->globalPosition(),
                event->button(), event->buttons(), event->modifiers());
            QApplication::sendEvent(child, &forwarded);
        }
    }
    event->accept();
}

void GridTrackOverlay::mouseMoveEvent(QMouseEvent *event)
{
    QString icao24 = hitTest(event->pos());
    setCursor(icao24.isEmpty() ? Qt::ArrowCursor : Qt::PointingHandCursor);

    // Forward move events to the parent so coordinate display keeps working
    QWidget *parent = parentWidget();
    if (parent)
    {
        QPointF parentPos = mapToParent(event->pos());
        QMouseEvent forwarded(
            event->type(), parentPos, event->globalPosition(),
            event->button(), event->buttons(), event->modifiers());
        QApplication::sendEvent(parent, &forwarded);
    }
}

void GridTrackOverlay::paintEvent(QPaintEvent *event)
{
    if (parentWidget())
        setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());

    QSize pixmapSize = _projection.pixmapSize();

    if (pixmapSize.isEmpty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    static QImage trackIcon(":/icons/track.png");
    static QImage scaledTrackIcon = trackIcon.scaled(
        ICON_SIZE,
        ICON_SIZE,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    for (const Track &track : _tracks)
    {
        QPointF widgetPos = trackWidgetPos(track);

        bool isSelected = (track.getIcao24() == _selectedIcao24);

        if (isSelected)
        {
            painter.save();
            painter.translate(widgetPos);
            QPen pen(QColor(0, 180, 255), 2);
            painter.setPen(pen);
            painter.setBrush(QColor(0, 180, 255, 40));
            painter.drawEllipse(QPointF(0, 0), HIT_RADIUS, HIT_RADIUS);
            painter.restore();
        }

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