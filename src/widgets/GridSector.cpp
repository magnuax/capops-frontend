#include "widgets/GridSector.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>

GridSector::GridSector(int row, int col, QWidget *parent)
    : QFrame(parent), _row(row), _col(col)
{
    setFrameStyle(QFrame::NoFrame);
    setLineWidth(1);
    setMinimumSize(24, 24);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GridSector::setState(SectorState state)
{
    _state = state;
    update();
}

void GridSector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit selected(_row, _col);
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
    QFrame::paintEvent(event);
    QPainter painter(this);

    QRect cell = rect().adjusted(1, 1, -1, -1);

    int alpha = 64;
    QColor fillColor;
    switch (_state)
    {
    case NORMAL:
        painter.fillRect(cell, QColor(0, 255, 0, alpha)); // green
        fillColor = QColor(0, 255, 0, alpha);
        break;

    case AT_RISK:
        painter.fillRect(cell, QColor(255, 160, 0, alpha)); // orange
        fillColor = QColor(255, 160, 0, alpha);
        break;

    case CONGESTED:
        painter.fillRect(cell, QColor(255, 0, 0, alpha)); // red
        fillColor = QColor(255, 0, 0, alpha);
        break;

    // !! PLACEHOLDER !!
    default:
        painter.fillRect(cell, QColor(0, 255, 0, alpha)); // green
        fillColor = QColor(0, 255, 0, alpha);
        break;
    }
    
    QPen pen(fillColor.darker(160));
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    QRectF cellBorder = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    painter.drawRect(cellBorder);
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