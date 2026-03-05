#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>

#include "panels/StateGridPanel.hpp"
#include "widgets/GridSector.hpp"

StateGridPanel::StateGridPanel(int numRows, int numCols, QWidget *parent) : QFrame(parent), _numRows(numRows), _numCols(numCols)
{
    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);
    outer->addWidget(createGrid(), 0, Qt::AlignCenter);
}

QSize StateGridPanel::getGridSize() const
{
    return _gridWidget->size();
}

void StateGridPanel::setMapSource(const QPixmap *mapSource)
{
    _mapSource = *mapSource;
    update();
}

QWidget *StateGridPanel::createGrid()
{
    _gridWidget = new QWidget(this);
    _gridWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _gridLayout = new QGridLayout(_gridWidget);
    _gridLayout->setContentsMargins(0, 0, 0, 0);
    _gridLayout->setSpacing(0);

    _cells.reserve(_numRows * _numCols);

    for (int row = 0; row < _numRows; ++row)
    {
        for (int col = 0; col < _numCols; ++col)
        {
            auto *cell = new GridSector(row, col, _gridWidget);

            connect(cell, &GridSector::selected, this, [this, cell](int row, int col)
                    { 
                        if (_selectedCell && _selectedCell != cell)
                        {
                            _selectedCell->setSelected(false);
                        }

                        _selectedCell = cell;
                        _selectedCell->setSelected(true);

                        emit sectorSelected(row, col); });

            _gridLayout->addWidget(cell, row, col);
            _cells.push_back(cell);
        }
    }

    return _gridWidget;
}

void StateGridPanel::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    const int availWidth = contentsRect().width();
    const int availHeight = contentsRect().height();

    const int cellSize = std::min(
        availWidth / _numCols,
        availHeight / _numRows);

    int newWidth = cellSize * _numCols;
    int newHeight = cellSize * _numRows;

    if (newWidth > newHeight)
    {
        _gridWidget->resize(newHeight, newHeight);
    }
    else
    {
        _gridWidget->resize(newWidth, newWidth);
    }

    _gridWidget->move(
        (availWidth - _gridWidget->width()) / 2,
        (availHeight - _gridWidget->height()) / 2);
}

void StateGridPanel::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (_mapSource.isNull())
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    const QRect gridRectangle = _gridWidget->geometry();

    QPixmap scaled = _mapSource.scaled(
        gridRectangle.size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation);

    const int x = (scaled.width() - gridRectangle.width()) / 2;
    const int y = (scaled.height() - gridRectangle.height()) / 2;

    painter.drawPixmap(
        gridRectangle,
        scaled,
        QRect(x, y, gridRectangle.width(), gridRectangle.height()));
}