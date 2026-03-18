#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>

#include "widgets/GridSector.hpp"

GridSector::GridSector(int row, int col, QWidget *parent)
    : QFrame(parent), _row(row), _col(col)
{
    // --- default states ---
    _riskState = RiskState::NORMAL;
    _weatherState = WeatherState::OK;
    _trafficState = TrafficState::LIGHT;
    _displayMode = DisplayMode::RISK;
    _isSelected = false;

    // --- style ---
    setFrameStyle(QFrame::NoFrame);
    setLineWidth(1);
    setMinimumSize(24, 24);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int GridSector::getRow() const
{
    return _row;
}

int GridSector::getCol() const
{
    return _col;
}

void GridSector::setRiskState(RiskState state)
{
    _riskState = state;
    update();
}

void GridSector::setWeatherState(WeatherState state)
{
    _weatherState = state;
    update();
}

void GridSector::setTrafficState(TrafficState state)
{
    _trafficState = state;
    update();
}

void GridSector::setDisplayMode(DisplayMode mode)
{
    _displayMode = mode;
    update();
}

void GridSector::setSelected(bool selected)
{
    _isSelected = selected;
    update();
}

void GridSector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit selected(this);
}

void GridSector::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction("Lorem ipsum");
    menu.addAction("Dolor sit amet");
    menu.exec(event->globalPos());
}

void GridSector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    const QRect cell = rect();

    QColor fillColor;
    switch (_displayMode)
    {
    case DisplayMode::RISK:
        fillColor = getRiskStateColor();
        break;
    case DisplayMode::WEATHER:
        fillColor = getWeatherStateColor();
        break;
    case DisplayMode::TRAFFIC:
        fillColor = getTrafficStateColor();
        break;
    case DisplayMode::NONE:
        fillColor = QColor(255, 255, 255, 0);
        break;
    }

    painter.fillRect(cell, fillColor);

    QPen pen(_isSelected ? Qt::black : QColor(0, 0, 0, 16));
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    QRectF cellBorder = QRectF(cell).adjusted(0.5, 0.5, -0.5, -0.5);
    painter.drawRect(cellBorder);
}

QColor GridSector::getRiskStateColor()
{
    int alpha = 48;

    switch (_riskState)
    {
    case RiskState::NORMAL:
        return QColor(0, 255, 0, alpha); // green

    case RiskState::AT_RISK:
        return QColor(255, 160, 0, alpha); // orange

    case RiskState::CONGESTED:
        return QColor(255, 0, 0, alpha); // red
    }

    QColor fallback(0, 0, 0, 0);
    return fallback;
}

QColor GridSector::getWeatherStateColor()
{
    int alpha = 48;

    switch (_weatherState)
    {
    case WeatherState::OK:
        return QColor(0, 255, 255, alpha); // cyan
    case WeatherState::DEGRADED:
        return QColor(255, 255, 0, alpha); // yellow
    case WeatherState::SEVERE:
        return QColor(255, 0, 0, alpha); // red
    case WeatherState::EXTREME:
        return QColor(128, 0, 128, alpha); // purple
    }

    QColor fallback(0, 0, 0, 0);
    return fallback;
}

QColor GridSector::getTrafficStateColor()
{
    int alpha = 48;

    switch (_trafficState)
    {
    case TrafficState::LIGHT:
        return QColor(0, 255, 0, alpha); // green
    case TrafficState::MODERATE:
        return QColor(255, 255, 0, alpha); // yellow
    case TrafficState::HEAVY:
        return QColor(255, 0, 0, alpha); // red
    }

    QColor fallback(0, 0, 0, 0);
    return fallback;
}

void GridSector::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    int newWidth = event->size().width();
    int newHeight = event->size().height();

    if (newWidth > newHeight)
    {
        QWidget::resize(newHeight, newHeight);
    }
    else
    {
        QWidget::resize(newWidth, newWidth);
    }
}
