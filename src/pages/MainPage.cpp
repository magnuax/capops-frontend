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
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);
    mainLayout->setStretch(2, 1);

    setLayout(mainLayout);

    wireConnections();

    QTimer::singleShot(0, this, [this]
                       { requestMap(60.1986, 11.1130, 13); });
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
            this, [](const QString &err)
            { qWarning() << err; });

    // --- Refresh UI on data reload ---
    connect(_dataEvents, &IFlightDataEvents::dataReloaded,
            _gridPanel, &StateGridPanel::refresh);

    connect(_dataEvents, &IFlightDataEvents::dataReloaded,
            _sectorDetailsPanel, &SectorDetailsPanel::refresh);
}

QWidget *MainPage::buildStateGrid()
{
    _gridPanel = new StateGridPanel(_dataService, this);
    _gridPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _gridPanel->setMinimumWidth(400);

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
    return _sectorDetailsPanel;
}

void MainPage::requestMap(double lat, double lon, int zoom)
{

    ITileMapService::Request request;
    request.coords = {lon, lat};
    request.zoom = zoom;
    request.imageSize = {800, 800};

    _mapFetcher->fetch(request);
}