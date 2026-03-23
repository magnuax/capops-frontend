#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QStandardPaths>

#include <QSslSocket>
#include <QToolButton>
#include <QButtonGroup>
#include <QDebug>

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

    // Skip map requests until bounds are populated from real data.
    if (minLat >= maxLat || minLon >= maxLon)
        return;

    // Avoid unnecessary tile fetches when bounds did not change.
    if (minLat == _lastRequestedMinLat &&
        maxLat == _lastRequestedMaxLat &&
        minLon == _lastRequestedMinLon &&
        maxLon == _lastRequestedMaxLon)
        return;

    _lastRequestedMinLat = minLat;
    _lastRequestedMaxLat = maxLat;
    _lastRequestedMinLon = minLon;
    _lastRequestedMaxLon = maxLon;

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