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

#include "services/ITileMapService.hpp"
#include "services/IFlightDataService.hpp"
#include "widgets/SegmentedControl.hpp"

MainPage::MainPage(IFlightDataService &dataService, ITileMapService *mapFetcher, QWidget *parent)
    : QWidget(parent),
      _mapFetcher(mapFetcher),
      _dataService(dataService)
{
    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->setContentsMargins(0, 0, 0, 0);
    displayLayout->setSpacing(0);
    displayLayout->addWidget(createStateGrid());
    displayLayout->addWidget(createDisplayControls(), 0, Qt::AlignCenter);

    QWidget *sectorDetailsPanel = createSectorDetailsPanel();
    QWidget *alertPanel = createAlertPanel();

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

    // --- Display controls <-> Grid panel ---
    connect(_displayControls, &SegmentedControl::segmentSelected, this, [this](int idx)
            {
        QVariant displayMode = _displayControls->getSegmentData(idx);
        if (displayMode.isValid())
        {
            _gridPanel->setDisplayMode(static_cast<DisplayMode>(displayMode.toInt()));
        } });

    // --- Sector details <-> Grid panel ---
    connect(_gridPanel, &StateGridPanel::sectorSelected, _sectorDetailsPanel, [this](int row, int col)
            { _sectorDetailsPanel->setSector(row, col); });

    // --- Map fetcher signals ---
    connect(_mapFetcher, &ITileMapService::finished, this, [this](const QPixmap &pm)
            { _gridPanel->setMapSource(pm); });

    connect(_mapFetcher, &ITileMapService::failed, this, [](const QString &err)
            { qWarning() << err; });
}

QWidget *MainPage::createStateGrid()
{
    _gridPanel = new StateGridPanel(_dataService, this);
    _gridPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return _gridPanel;
};

QWidget *MainPage::createAlertPanel()
{
    _alertPanel = new AlertPanel(this);
    return _alertPanel;
}

QWidget *MainPage::createSectorDetailsPanel()
{
    _sectorDetailsPanel = new SectorDetailsPanel(_dataService, this);
    return _sectorDetailsPanel;
}

QWidget *MainPage::createDisplayControls()
{

    _displayControls = new SegmentedControl({"Risk", "Weather", "Traffic", "None"}, this);

    _displayControls->setSegmentData(0, static_cast<int>(DisplayMode::RISK));
    _displayControls->setSegmentData(1, static_cast<int>(DisplayMode::WEATHER));
    _displayControls->setSegmentData(2, static_cast<int>(DisplayMode::TRAFFIC));
    _displayControls->setSegmentData(3, static_cast<int>(DisplayMode::NONE));

    return _displayControls;
}

void MainPage::requestMap(double lat, double lon, int zoom)
{

    ITileMapService::Request request;
    request.coords = {lon, lat};
    request.zoom = zoom;
    request.imageSize = {800, 800};

    _mapFetcher->fetch(request);
}