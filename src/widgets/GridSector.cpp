#include "widgets/GridSector.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>

GridSector::GridSector(int row, int col, QWidget *parent)
    : QFrame(parent), _row(row), _col(col)
{
    setFrameStyle(QFrame::Box);
    setLineWidth(1);
    setMinimumSize(48, 48);
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
        emit clicked(_row, _col);
}

void GridSector::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction("Rename");
    menu.addAction("Delete");
    menu.exec(event->globalPos());
}

void GridSector::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e); // draws the frame

    QPainter p(this);
    // fill interior (don’t paint over the border too aggressively)
    QRect inner = rect().adjusted(1, 1, -1, -1);

    // pick fill based on state
    // (choose your own colors)

    switch (_state)
    {
    case NORMAL:
        p.fillRect(inner, QColor(200, 255, 200)); // light green
        break;
    case CONGESTED:
        p.fillRect(inner, QColor(255, 200, 200)); // light red
        break;
    case AT_RISK:
        p.fillRect(inner, QColor(255, 165, 0)); // orange
        break;
    }
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