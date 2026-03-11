#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QButtonGroup>
#include <QToolButton>
#include <QMargins>
#include <QTabBar>

#include "panels/StateGridPanel.hpp"
#include "widgets/GridCanvas.hpp"
#include "widgets/GridSector.hpp"
#include "services/interfaces/IFlightDataService.hpp"

StateGridPanel::StateGridPanel(IFlightDataService &dataService, QWidget *parent) : QFrame(parent), _dataService(dataService)
{
    _numRows = _dataService.getRowCount();
    _numCols = _dataService.getColCount();

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 8);
    outer->setSpacing(0);

    _tabBar = new QTabBar(this);
    _tabBar->addTab(QIcon(":/icons/tab-risk.png"), "Risk");
    _tabBar->addTab(QIcon(":/icons/tab-weather.png"), "Weather");
    _tabBar->addTab(QIcon(":/icons/tab-traffic.png"), "Traffic");
    _tabBar->addTab("None");

    _tabBar->setExpanding(false);
    _tabBar->setTabData(0, static_cast<int>(DisplayMode::RISK));
    _tabBar->setTabData(1, static_cast<int>(DisplayMode::WEATHER));
    _tabBar->setTabData(2, static_cast<int>(DisplayMode::TRAFFIC));
    _tabBar->setTabData(3, static_cast<int>(DisplayMode::NONE));

    outer->addWidget(_tabBar, 0, Qt::AlignLeft);
    auto *gridContainer = new QWidget(this);
    gridContainer->setObjectName("GridContainer");
    gridContainer->setContentsMargins(12, 12, 12, 12);

    auto *gridContainerLayout = new QVBoxLayout(gridContainer);
    gridContainerLayout->addWidget(buildGrid(), 0, Qt::AlignCenter);

    outer->addWidget(gridContainer, 1);

    _tabBar->setCurrentIndex(0);
    setDisplayMode(DisplayMode::RISK);

    connect(_tabBar, &QTabBar::currentChanged,
            this, &StateGridPanel::handleTabChange);
}

void StateGridPanel::refresh()
{
    for (GridSector *cell : _cells)
    {
        int sectorId = _dataService.getSectorId(cell->getRow(), cell->getCol());

        cell->setRiskState(_dataService.getRisk(sectorId));
        cell->setWeatherState(_dataService.getWeather(sectorId));
        cell->setTrafficState(_dataService.getTraffic(sectorId));
    }
}

void StateGridPanel::setDisplayMode(DisplayMode mode)
{
    for (GridSector *cell : _cells)
    {
        cell->setDisplayMode(mode);
    }
}

void StateGridPanel::setMapSource(const QPixmap &mapSource)
{
    _mapSource = mapSource;
    if (auto *canvas = qobject_cast<GridCanvas *>(_gridWidget))
    {
        canvas->setMapSource(mapSource);
    }
}

void StateGridPanel::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    QRect area = _gridWidget->parentWidget()->contentsRect();

    const int availWidth = area.width();
    const int availHeight = area.height();

    const int cellSize = std::min(
        availWidth / _numCols,
        availHeight / _numRows);

    int newWidth = cellSize * _numCols;
    int newHeight = cellSize * _numRows;

    _gridWidget->resize(cellSize * _numCols, cellSize * _numRows);

    _gridWidget->move(
        area.x() + (availWidth - _gridWidget->width()) / 2,
        area.y() + (availHeight - _gridWidget->height()) / 2);
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
            int sectorId = _dataService.getSectorId(row, col);

            auto *cell = new GridSector(row, col, _gridWidget);
            connect(cell, &GridSector::selected, this, &StateGridPanel::handleSectorSelection);

            cell->setRiskState(_dataService.getRisk(sectorId));
            cell->setWeatherState(_dataService.getWeather(sectorId));
            cell->setTrafficState(_dataService.getTraffic(sectorId));

            _gridLayout->addWidget(cell, row, col);
            _cells.push_back(cell);
        }
    }

    return _gridWidget;
}

void StateGridPanel::handleSectorSelection(GridSector *cell)
{
    int row = cell->getRow();
    int col = cell->getCol();

    if (_selectedCell && _selectedCell != cell)
    {
        _selectedCell->setSelected(false);
    }

    _selectedCell = cell;
    _selectedCell->setSelected(true);

    int sectorId = _dataService.getSectorId(row, col);

    emit sectorSelected(sectorId);
}

void StateGridPanel::handleTabChange(int tabIndex)
{
    QVariant data = _tabBar->tabData(tabIndex);

    if (!data.isValid())
        return;

    setDisplayMode(static_cast<DisplayMode>(data.toInt()));
}