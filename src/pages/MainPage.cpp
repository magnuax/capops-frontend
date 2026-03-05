#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QStandardPaths>

#include <QSslSocket>
#include <QDebug>

#include "pages/MainPage.hpp"
#include "panels/StateGridPanel.hpp"
#include "panels/OperatorPanel.hpp"
#include "panels/SectorDetailsPanel.hpp"

#include "helpers/TileMapFetcherCARTO.hpp"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(createSectorDetailsPanel());
    mainLayout->addWidget(createStateGrid());
    mainLayout->addWidget(createOperatorPanel());

    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);
    mainLayout->setStretch(2, 1);

    setLayout(mainLayout);

    _mapFetcher = new TileMapFetcherCARTO(this);
    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/osmtiles");

    wireConnections();

    QTimer::singleShot(0, this, [this]
                       { requestMap(60.1986, 11.1130, 13); });
}

void MainPage::wireConnections()
{
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