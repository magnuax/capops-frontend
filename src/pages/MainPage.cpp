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
#include "services/interfaces/IFlightDataEvents.hpp"

MainPage::MainPage(
    IFlightDataService &dataService,
    IFlightDataEvents *dataEvents,
    ITileMapService *mapFetcher,
    QWidget *parent)
    : QWidget(parent),
      _mapFetcher(mapFetcher),
      _dataService(dataService),
      _dataEvents(dataEvents)
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

    // --- Refresh UI on data reload ---
    connect(_dataEvents, &IFlightDataEvents::dataReloaded,
            this, &MainPage::refresh);
}

void MainPage::refresh()
{
    _gridPanel->refresh();
    _sectorDetailsPanel->refresh();
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
    _alertPanel = new AlertPanel(this);
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
    SectorSummaryData data = _dataService.getSectorSummaryData();

    ITileMapService::Request request;
    request.minLat = data.getMinLat();
    request.maxLat = data.getMaxLat();
    request.minLon = data.getMinLon();
    request.maxLon = data.getMaxLon();

    _mapFetcher->fetch(request);
}

void MainPage::onMapFetchFailed(const QString &error)
{
    qWarning() << "Map fetch failed:" << error;
}