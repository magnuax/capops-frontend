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
#include "panels/OperatorPanel.hpp"
#include "panels/SectorDetailsPanel.hpp"

#include "helpers/TileMapFetcherCARTO.hpp"
#include "widgets/SegmentedControl.hpp"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->setContentsMargins(0, 0, 0, 0);
    displayLayout->setSpacing(0);
    displayLayout->addWidget(createStateGrid());
    displayLayout->addWidget(createDisplayControls(), 0, Qt::AlignCenter);

    QWidget *sectorDetailsPanel = createSectorDetailsPanel();
    QWidget *operatorPanel = createOperatorPanel();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sectorDetailsPanel);
    mainLayout->addLayout(displayLayout);
    mainLayout->addWidget(operatorPanel);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);
    mainLayout->setStretch(2, 1);

    _mapFetcher = new TileMapFetcherCARTO(this);
    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");

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
    connect(_mapFetcher, &TileMapFetcherCARTO::finished, this, [this](const QPixmap &pm)
            { _gridPanel->setMapSource(&pm); });

    connect(_mapFetcher, &TileMapFetcherCARTO::failed, this, [](const QString &err)
            { qWarning() << err; });
}

QWidget *MainPage::createStateGrid()
{
    int PLACEHOLDER_rows = 30;
    int PLACEHOLDER_cols = 30;

    _gridPanel = new StateGridPanel(
        PLACEHOLDER_rows,
        PLACEHOLDER_cols,
        this);

    _gridPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return _gridPanel;
};

QWidget *MainPage::createOperatorPanel()
{
    _operatorPanel = new OperatorPanel(this);
    return _operatorPanel;
}

QWidget *MainPage::createSectorDetailsPanel()
{
    _sectorDetailsPanel = new SectorDetailsPanel(this);
    return _sectorDetailsPanel;
}

QWidget *MainPage::createDisplayControls()
{

    _displayControls = new SegmentedControl({"Risk", "Weather", "Traffic"}, this);

    _displayControls->setSegmentData(0, static_cast<int>(DisplayMode::RISK));
    _displayControls->setSegmentData(1, static_cast<int>(DisplayMode::WEATHER));
    _displayControls->setSegmentData(2, static_cast<int>(DisplayMode::TRAFFIC));

    return _displayControls;
}

void MainPage::requestMap(double lat, double lon, int zoom)
{
    QSize size = _gridPanel->getGridSize();

    if (size.isEmpty())
    {
        size = QSize(600, 600);
    }

    TileMapFetcherCARTO::Request request;
    request.coords = {lon, lat};
    request.zoom = zoom;
    request.imageSize = size;

    _mapFetcher->fetch(request);
}