#include <algorithm>
#include <cmath>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QTabBar>
#include <QLabel>

#include "panels/StateGridPanel.hpp"
#include "widgets/GridCanvas.hpp"
#include "widgets/GridSector.hpp"
#include "widgets/GridTrackOverlay.hpp"
#include "services/interfaces/IFlightDataService.hpp"

StateGridPanel::StateGridPanel(IFlightDataService *dataService, QWidget *parent)
    : QFrame(parent),
      _dataService(dataService)
{
    buildPanel();
    wireConnections();
}

void StateGridPanel::wireConnections()
{
    connect(_tabBar, &QTabBar::currentChanged,
            this, &StateGridPanel::handleTabChange);

    connect(_gridWidget, &GridCanvas::cursorLatLon,
            this, &StateGridPanel::updateCoordinateLabel);

    connect(_trackOverlay, &GridTrackOverlay::trackSelected,
            this, &StateGridPanel::trackSelected);
}

void StateGridPanel::buildPanel()
{
    const SectorSummaryData data = _dataService->getSectorSummaryData();
    _numRows = data.getRowCount();
    _numCols = data.getColCount();

    auto *tabs = buildTabLayout();
    auto *grid = buildGridContainer(data);

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(PANEL_OUTER_MARGINS);
    outer->setSpacing(0);
    outer->addLayout(tabs);
    outer->addWidget(grid, 1);

    _tabBar->setCurrentIndex(0);
    setDisplayMode(DisplayMode::RISK);
}

QHBoxLayout *StateGridPanel::buildTabLayout()
{
    _tabBar = buildTabWidget();
    _coordLabel = new QLabel("", this);
    _coordLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _coordLabel->setObjectName("coordLabel");

    auto *tabRow = new QHBoxLayout();
    tabRow->setContentsMargins(0, 0, 0, 0);
    tabRow->setSpacing(0);
    tabRow->addWidget(_tabBar, 0, Qt::AlignLeft | Qt::AlignBottom);
    tabRow->addStretch();
    tabRow->addWidget(_coordLabel, 0, Qt::AlignRight | Qt::AlignVCenter);

    return tabRow;
}

QTabBar *StateGridPanel::buildTabWidget()
{
    auto *tabBar = new QTabBar(this);
    tabBar->addTab(QIcon(":/icons/tab-risk.png"), "Risk");
    tabBar->addTab(QIcon(":/icons/tab-weather.png"), "Weather");
    tabBar->addTab(QIcon(":/icons/tab-traffic.png"), "Traffic");
    tabBar->addTab("None");
    tabBar->setExpanding(false);
    tabBar->setTabData(TAB_IDX_RISK, static_cast<int>(DisplayMode::RISK));
    tabBar->setTabData(TAB_IDX_WEATHER, static_cast<int>(DisplayMode::WEATHER));
    tabBar->setTabData(TAB_IDX_TRAFFIC, static_cast<int>(DisplayMode::TRAFFIC));
    tabBar->setTabData(TAB_IDX_NONE, static_cast<int>(DisplayMode::NONE));

    return tabBar;
}

QWidget *StateGridPanel::buildGridContainer(const SectorSummaryData &data)
{
    auto *container = new QWidget(this);
    container->setObjectName("GridContainer");
    container->setContentsMargins(GRID_CONTAINER_MARGINS);

    auto *layout = new QVBoxLayout(container);
    layout->addWidget(buildGridWidget(data), 0, Qt::AlignCenter);

    return container;
}

GridCanvas *StateGridPanel::buildGridWidget(const SectorSummaryData &data)
{
    _gridWidget = new GridCanvas(this);
    _gridWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _gridWidget->setMouseTracking(true);

    _trackOverlay = new GridTrackOverlay(_gridWidget);
    _trackOverlay->setGeometry(_gridWidget->rect());
    _trackOverlay->raise();

    _gridLayout = new QGridLayout(_gridWidget);
    _gridLayout->setContentsMargins(0, 0, 0, 0);
    _gridLayout->setSpacing(0);

    _cells.reserve(_numRows * _numCols);

    for (int row = 0; row < _numRows; ++row)
    {
        for (int col = 0; col < _numCols; ++col)
        {
            SectorSummary summary = data.getSectorSummary(row, col);

            auto *cell = new GridSector(row, col, summary.getSectorId(), _gridWidget);

            connect(cell, &GridSector::selected,
                    this, &StateGridPanel::handleSectorSelection);

            cell->setRiskState(summary.getRiskState());
            cell->setWeatherState(summary.getWeatherState());
            cell->setTrafficState(summary.getTrafficState());

            cell->setMouseTracking(true);

            _gridLayout->addWidget(cell, (_numRows - 1 - row), col);
            _cells.push_back(cell);
        }
    }

    _trackOverlay->raise();

    return _gridWidget;
}

void StateGridPanel::refresh()
{
    const SectorSummaryData data = _dataService->getSectorSummaryData();

    if (_numRows == 0 && _numCols == 0 && data.getRowCount() > 0 && data.getColCount() > 0)
    {
        rebuildGrid(data);
    }

    for (GridSector *cell : _cells)
    {
        const SectorSummary summary = data.getSectorSummary(cell->getRow(), cell->getCol());

        cell->setRiskState(summary.getRiskState());
        cell->setWeatherState(summary.getWeatherState());
        cell->setTrafficState(summary.getTrafficState());
    }

    if (_trackOverlay)
    {
        _trackOverlay->setTracks(_dataService->getTrackData().getTracks());
    }
}

void StateGridPanel::rebuildGrid(const SectorSummaryData &data)
{
    _numRows = data.getRowCount();
    _numCols = data.getColCount();

    for (GridSector *cell : _cells)
    {
        _gridLayout->removeWidget(cell);
        delete cell;
    }
    _cells.clear();
    _selectedCell = nullptr;

    _cells.reserve(_numRows * _numCols);

    for (int row = 0; row < _numRows; ++row)
    {
        for (int col = 0; col < _numCols; ++col)
        {
            SectorSummary summary = data.getSectorSummary(row, col);

            auto *cell = new GridSector(row, col, summary.getSectorId(), _gridWidget);

            connect(cell, &GridSector::selected,
                    this, &StateGridPanel::handleSectorSelection);

            cell->setRiskState(summary.getRiskState());
            cell->setWeatherState(summary.getWeatherState());
            cell->setTrafficState(summary.getTrafficState());
            cell->setDisplayMode(static_cast<DisplayMode>(_tabBar->tabData(_tabBar->currentIndex()).toInt()));
            cell->setMouseTracking(true);

            _gridLayout->addWidget(cell, (_numRows - 1 - row), col);
            _cells.push_back(cell);
        }
    }

    if (_trackOverlay)
        _trackOverlay->raise();

    // Force layout recalculation so resizeEvent updates geometry
    QResizeEvent re(size(), size());
    resizeEvent(&re);
}

void StateGridPanel::setDisplayMode(DisplayMode mode)
{
    for (GridSector *cell : _cells)
    {
        cell->setDisplayMode(mode);
    }
}

void StateGridPanel::setMapSource(const QPixmap &mapSource, const MapProjection &projection)
{
    if (!_gridWidget || !_trackOverlay)
        return;

    _mapSource = mapSource;
    _gridWidget->setMapSource(mapSource);
    _trackOverlay->setProjection(projection);
    _gridWidget->setProjection(projection);

    QResizeEvent re(size(), size());
    resizeEvent(&re);
}

void StateGridPanel::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    if (!_gridWidget || !_trackOverlay || !_gridWidget->parentWidget())
        return;

    if (_numRows <= 0 || _numCols <= 0)
        return;

    QRect area = _gridWidget->parentWidget()->contentsRect();

    const int availWidth = area.width();
    const int availHeight = area.height();

    const int cellSize = std::min(
        availWidth / _numCols,
        availHeight / _numRows);

    _gridWidget->resize(
        cellSize * _numCols,
        cellSize * _numRows);


    _gridWidget->move(
        area.x() + (availWidth - _gridWidget->width()) / 2,
        area.y() + (availHeight - _gridWidget->height()) / 2);

    _trackOverlay->setGeometry(_gridWidget->rect());
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

    int sectorId = _dataService
                       ->getSectorSummaryData()
                       .getSectorSummary(row, col)
                       .getSectorId();

    emit sectorSelected(sectorId);
}

void StateGridPanel::handleTabChange(int tabIndex)
{
    const QVariant data = _tabBar->tabData(tabIndex);

    if (!data.isValid())
        return;

    setDisplayMode(static_cast<DisplayMode>(data.toInt()));
}

void StateGridPanel::updateCoordinateLabel(double lat, double lon)
{
    if (std::isnan(lat) || std::isnan(lon))
    {
        _coordLabel->setText("");
        return;
    }

    _coordLabel->setText(QString("Lat: %1\nLon: %2")
                             .arg(lat, 0, 'f', 4)
                             .arg(lon, 0, 'f', 4));
}

void StateGridPanel::selectTrack(const QString &icao24)
{
    if (_trackOverlay)
        _trackOverlay->setSelectedTrack(icao24);
}