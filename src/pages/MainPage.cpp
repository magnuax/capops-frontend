#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QStandardPaths>

#include <algorithm>

#include <QSslSocket>
#include <QToolButton>
#include <QButtonGroup>

#include "pages/MainPage.hpp"
#include "panels/StateGridPanel.hpp"
#include "panels/AlertPanel.hpp"
#include "panels/SectorDetailsPanel.hpp"

#include "services/interfaces/ITileMapService.hpp"
#include "services/interfaces/IFlightDataService.hpp"

MainPage::MainPage(
    IFlightDataService *dataService,
    ITileMapService *mapFetcher,
    QWidget *parent)
    : QWidget(parent),
      _mapFetcher(mapFetcher),
      _dataService(dataService)
{
    buildPage();
    wireConnections();
    refresh();

    QTimer::singleShot(0, this, &MainPage::requestMap);
}

void MainPage::wireConnections()
{
    // --- Sector details <-> Grid panel ---
    connect(_gridPanel, &StateGridPanel::sectorSelected,
            _sectorDetailsPanel, &SectorDetailsPanel::setSector);

    // --- Track selection: grid overlay <-> details panel ---
    connect(_gridPanel, &StateGridPanel::trackSelected,
            this, [this](const QString &icao24)
            {
                const auto sectors = _dataService->getSectorSummaryData().getSectorSummaries();

                for (const auto &sector : sectors)
                {
                    const auto &aircraftIds = sector.getAircraftIds();
                    const auto it = std::find(aircraftIds.begin(), aircraftIds.end(), icao24);

                    if (it != aircraftIds.end())
                    {
                        _gridPanel->selectSector(sector.getSectorId());
                        _sectorDetailsPanel->setSector(sector.getSectorId());
                        break;
                    }
                }

                _sectorDetailsPanel->selectAircraftById(icao24);
            });

    connect(_sectorDetailsPanel, &SectorDetailsPanel::aircraftSelected,
            _gridPanel, &StateGridPanel::selectTrack);

    // --- Map fetcher signals ---
    connect(_mapFetcher, &ITileMapService::finished,
            _gridPanel, &StateGridPanel::setMapSource);

    connect(_mapFetcher, &ITileMapService::failed,
            this, &MainPage::onMapFetchFailed);

    // --- Refresh UI and projection when websocket data updates ---
    connect(_dataService, &IFlightDataService::dataReloaded,
            this, [this]()
            {
                refreshRiskEvents();
                refresh();
                requestMap();
            });
}

void MainPage::refresh()
{
    _gridPanel->refresh();
    _sectorDetailsPanel->refresh();

    if (!_mapLoaded)
    {
        SectorSummaryData data = _dataService->getSectorSummaryData();
        if (data.getRowCount() > 0 && data.getColCount() > 0)
        {
            _mapLoaded = true;
            requestMap();
        }
    }
}

void MainPage::refreshRiskEvents()
{
    _alertPanel->setRiskEventData(_dataService->getRiskEventData());
}

void MainPage::buildPage()
{
    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->setContentsMargins(0, 0, 0, 0);
    displayLayout->setSpacing(0);
    displayLayout->addWidget(buildStateGrid());

    QWidget *sectorDetailsPanel = buildSectorDetailsPanel();
    QWidget *alertPanel = buildAlertPanel();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sectorDetailsPanel);
    mainLayout->addLayout(displayLayout);
    mainLayout->addWidget(alertPanel);
    mainLayout->setStretch(0, DETAILS_PANEL_STRETCH);
    mainLayout->setStretch(1, GRID_PANEL_STRETCH);
    mainLayout->setStretch(2, ALERT_PANEL_STRETCH);

    setLayout(mainLayout);
}

QWidget *MainPage::buildStateGrid()
{
    _gridPanel = new StateGridPanel(_dataService, this);
    _gridPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _gridPanel->setMinimumWidth(MIN_GRID_WIDTH);

    return _gridPanel;
};

QWidget *MainPage::buildAlertPanel()
{
    _alertPanel = new AlertPanel(_dataService, this);
    return _alertPanel;
}

QWidget *MainPage::buildSectorDetailsPanel()
{
    _sectorDetailsPanel = new SectorDetailsPanel(_dataService, this);
    _sectorDetailsPanel->setMinimumWidth(MIN_DETAILS_WIDTH);
    return _sectorDetailsPanel;
}

void MainPage::requestMap()
{
    SectorSummaryData data = _dataService->getSectorSummaryData();

    const double minLat = data.getMinLat();
    const double maxLat = data.getMaxLat();
    const double minLon = data.getMinLon();
    const double maxLon = data.getMaxLon();

    if (data.getRowCount() == 0 || data.getColCount() == 0)
        return;

    ITileMapService::Request request;
    request.minLat = minLat;
    request.maxLat = maxLat;
    request.minLon = minLon;
    request.maxLon = maxLon;

    _mapFetcher->fetch(request);
}

void MainPage::onMapFetchFailed(const QString &error)
{
    qWarning() << "Map fetch failed:" << error;
}