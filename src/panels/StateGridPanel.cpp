#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QButtonGroup>
#include <QToolButton>
#include <QTabWidget>

#include "panels/StateGridPanel.hpp"
#include "widgets/GridCanvas.hpp"
#include "widgets/GridSector.hpp"
#include "services/interfaces/IFlightDataService.hpp"

StateGridPanel::StateGridPanel(IFlightDataService &dataService, QWidget *parent) : QFrame(parent), _dataService(dataService)
{
    _numRows = _dataService.getRowCount();
    _numCols = _dataService.getColCount();

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    auto *tabs = new QTabWidget(this);
    outer->addWidget(tabs);

    auto *riskTab = new QWidget(this);
    auto *riskLayout = new QVBoxLayout(riskTab);

    riskLayout->addWidget(buildGrid(), 0, Qt::AlignCenter);

    tabs->addTab(riskTab, "Risk");
}

void StateGridPanel::setDisplayMode(DisplayMode mode)
{
    for (GridSector *cell : _cells)
    {
        cell->setDisplayMode(mode);
    }
}

QSize StateGridPanel::getGridSize() const
{
    return _gridWidget->size();
}

void StateGridPanel::setMapSource(const QPixmap &mapSource)
{
    _mapSource = mapSource;
    if (auto *canvas = qobject_cast<GridCanvas *>(_gridWidget))
    {
        canvas->setMapSource(mapSource);
    }
}

QWidget *StateGridPanel::buildGrid()
{
    _gridWidget = new GridCanvas(this);
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

            // PLACEHOLDER ID MAPPING: 
            int sectorId = row * _numCols + col;

            cell->setRiskState(_dataService.getRisk(sectorId));
            cell->setWeatherState(_dataService.getWeather(sectorId));
            cell->setTrafficState(_dataService.getTraffic(sectorId));

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