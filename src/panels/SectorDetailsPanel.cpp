#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QIcon>

#include <stdexcept>

#include "services/interfaces/IFlightDataService.hpp"
#include "panels/SectorDetailsPanel.hpp"
#include "domain/SectorStates.hpp"

SectorDetailsPanel::SectorDetailsPanel(IFlightDataService *dataService, QWidget *parent)
    : QWidget(parent),
      _dataService(dataService)
{
    const std::vector<SectorSummary> sectors = _dataService
                                                   ->getSectorSummaryData()
                                                   .getSectorSummaries();

    if (!sectors.empty())
    {
        _selectedSectorId = sectors.front().getSectorId();
        _selectedSectorIdx = QPoint(sectors.front().getRow(), sectors.front().getCol());
    }

    auto *mainLayout = new QVBoxLayout(this);

    auto *tabs = new QTabWidget(this);
    mainLayout->addWidget(tabs);

    auto *sectorDetails = new QWidget(this);
    auto *detailsLayout = new QVBoxLayout(sectorDetails);

    detailsLayout->addWidget(buildSectorStatusWidget());
    detailsLayout->addWidget(buildSelectedAircraftWidget());
    detailsLayout->addWidget(buildAircraftListWidget());

    tabs->addTab(sectorDetails, "Details");

    refresh();
}

QWidget *SectorDetailsPanel::buildSectorStatusWidget()
{
    _sectorStatusWidget = new QWidget(this);
    _sectorStatusWidget->setObjectName("SectorStatusWidget");

    auto *layout = new QVBoxLayout(_sectorStatusWidget);
    auto *header = new QLabel("Sector status:", _sectorStatusWidget);
    auto *grid = new QGridLayout();

    layout->addWidget(header);
    layout->addLayout(grid);

    QLabel *riskLabel = new QLabel("Risk:", _sectorStatusWidget);
    QLabel *weatherLabel = new QLabel("Weather:", _sectorStatusWidget);
    QLabel *trafficLabel = new QLabel("Traffic:", _sectorStatusWidget);

    _riskState = new QLabel("n/a", _sectorStatusWidget);
    _weatherState = new QLabel("n/a", _sectorStatusWidget);
    _trafficState = new QLabel("n/a", _sectorStatusWidget);

    _riskIcon = new QLabel(_sectorStatusWidget);
    _weatherIcon = new QLabel(_sectorStatusWidget);
    _trafficIcon = new QLabel(_sectorStatusWidget);

    grid->addWidget(riskLabel, 0, 0);
    grid->addWidget(_riskState, 0, 1);
    grid->addWidget(_riskIcon, 0, 2);

    grid->addWidget(weatherLabel, 1, 0);
    grid->addWidget(_weatherState, 1, 1);
    grid->addWidget(_weatherIcon, 1, 2);

    grid->addWidget(trafficLabel, 2, 0);
    grid->addWidget(_trafficState, 2, 1);
    grid->addWidget(_trafficIcon, 2, 2);

    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    grid->setColumnStretch(2, 0);
    grid->setColumnMinimumWidth(0, 40);

    updateSectorStatusWidget();

    return _sectorStatusWidget;
}

QWidget *SectorDetailsPanel::buildAircraftListWidget()
{
    _aircraftListWidget = new QWidget(this);
    _aircraftListWidget->setLayout(new QVBoxLayout());
    _aircraftListWidget->setObjectName("AircraftListWidget");

    _aircraftEntries = new QListWidget(_aircraftListWidget);
    _aircraftEntries->setFocusPolicy(Qt::NoFocus);
    _aircraftEntries->setCursor(Qt::PointingHandCursor);
    _aircraftEntries->setSelectionMode(QAbstractItemView::SingleSelection);

    _aircraftListHeader = new QLabel(QString("Aircrafts in sector ID-%1").arg(_selectedSectorId), _aircraftListWidget);
    _aircraftListWidget->layout()->addWidget(_aircraftListHeader);
    _aircraftListWidget->layout()->addWidget(_aircraftEntries);

    connect(_aircraftEntries, &QListWidget::itemClicked,
            this, &SectorDetailsPanel::selectAircraft);

    return _aircraftListWidget;
}

QWidget *SectorDetailsPanel::buildSelectedAircraftWidget()
{
    _selectedAircraftWidget = new QWidget(this);
    _selectedAircraftWidget->setObjectName("SelectedAircraftWidget");
    _selectedAircraftHeader = new QLabel("No aircraft selected", _selectedAircraftWidget);

    auto *layout = new QVBoxLayout(_selectedAircraftWidget);
    auto *grid = new QGridLayout();

    layout->addWidget(_selectedAircraftHeader);
    layout->addLayout(grid);

    QLabel *latitudeLabel = new QLabel("latitude:", _selectedAircraftWidget);
    QLabel *longitudeLabel = new QLabel("longitude:", _selectedAircraftWidget);
    QLabel *altitudeLabel = new QLabel("altitude:", _selectedAircraftWidget);
    QLabel *groundSpeedLabel = new QLabel("ground speed:", _selectedAircraftWidget);
    QLabel *vertRateLabel = new QLabel("vertical rate:", _selectedAircraftWidget);
    QLabel *headingLabel = new QLabel("heading:", _selectedAircraftWidget);
    QLabel *groundTrackLabel = new QLabel("ground track:", _selectedAircraftWidget);

    _aircraftLatitude = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftLongitude = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftAltitude = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftGroundSpeed = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftVertRate = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftHeading = new QLabel("n/a", _selectedAircraftWidget);
    _aircraftGroundTrack = new QLabel("n/a", _selectedAircraftWidget);

    grid->addWidget(latitudeLabel, 0, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftLatitude, 0, 1, Qt::AlignRight);

    grid->addWidget(longitudeLabel, 1, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftLongitude, 1, 1, Qt::AlignRight);

    grid->addWidget(altitudeLabel, 2, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftAltitude, 2, 1, Qt::AlignRight);

    grid->addWidget(groundSpeedLabel, 3, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftGroundSpeed, 3, 1, Qt::AlignRight);

    grid->addWidget(vertRateLabel, 4, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftVertRate, 4, 1, Qt::AlignRight);

    grid->addWidget(headingLabel, 5, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftHeading, 5, 1, Qt::AlignRight);

    grid->addWidget(groundTrackLabel, 6, 0, Qt::AlignLeft);
    grid->addWidget(_aircraftGroundTrack, 6, 1, Qt::AlignRight);

    grid->setColumnMinimumWidth(0, 40);

    updateSelectedAircraftWidget();

    return _selectedAircraftWidget;
}

void SectorDetailsPanel::updateSectorStatusWidget()
{
    if (_selectedSectorId < 0)
    {
        _riskState->setText("n/a");
        _weatherState->setText("n/a");
        _trafficState->setText("n/a");

        _riskIcon->clear();
        _weatherIcon->clear();
        _trafficIcon->clear();
        return;
    }

    SectorSummary sector = _dataService
                               ->getSectorSummaryData()
                               .getSectorSummary(_selectedSectorId);

    RiskState risk = sector.getRiskState();
    WeatherState weather = sector.getWeatherState();
    TrafficState traffic = sector.getTrafficState();

    _riskState->setText(toString(risk));
    _weatherState->setText(toString(weather));
    _trafficState->setText(toString(traffic));

    _riskIcon->setPixmap(loadStatusIcon(toIconPath(risk)));
    _weatherIcon->setPixmap(loadStatusIcon(toIconPath(weather)));
    _trafficIcon->setPixmap(loadStatusIcon(toIconPath(traffic)));
}

QPixmap SectorDetailsPanel::loadStatusIcon(const QString &iconPath)
{
    QPixmap icon(iconPath);

    if (icon.isNull())
    {
        return QPixmap();
    }

    QPixmap scaled = icon.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return scaled;
}

void SectorDetailsPanel::updateAircraftListWidget()
{
    std::vector<QString> flights = _dataService
                                       ->getSectorSummaryData()
                                       .getSectorSummary(_selectedSectorId)
                                       .getAircraftIds();

    _aircraftEntries->clear();
    _aircraftListHeader->setText(QString("Aircrafts in sector ID-%1").arg(_selectedSectorId));

    for (const QString &icao24 : flights)
    {
        auto *item = new QListWidgetItem(icao24, _aircraftEntries);
        item->setData(Qt::UserRole, icao24);
    }
}

void SectorDetailsPanel::updateSelectedAircraftWidget()
{
    QString headerText = "No aircraft selected";

    _aircraftLatitude->setText("n/a");
    _aircraftLongitude->setText("n/a");
    _aircraftAltitude->setText("n/a");
    _aircraftGroundSpeed->setText("n/a");
    _aircraftVertRate->setText("n/a");
    _aircraftHeading->setText("n/a");
    _aircraftGroundTrack->setText("n/a");

    if (!_selectedAircraftId.isEmpty())
    {
        headerText = QString("Selected aircraft: %1").arg(_selectedAircraftId);
    }

    _selectedAircraftHeader->setText(headerText);

    _selectedAircraftHeader->setText(QString("Selected aircraft: %1").arg(_selectedAircraftId));

    const std::vector<Track> &tracks = _dataService->getTrackData().getTracks();

    auto it = std::find_if(tracks.begin(), tracks.end(), [&](const Track &t)
                           { return t.getIcao24() == _selectedAircraftId; });

    if (it == tracks.end())
        return;

    const Track &track = *it;

    _aircraftLatitude->setText(QString("%1°")
                                   .arg(track.getPosition()[0], 0, 'f', 4));

    _aircraftLongitude->setText(QString("%1°")
                                    .arg(track.getPosition()[1], 0, 'f', 4));

    _aircraftAltitude->setText(QString("%1 ft")
                                   .arg(track.getPosition()[2], 0, 'f', 0));

    _aircraftGroundSpeed->setText(QString("%1 kts")
                                      .arg(track.getVelocity()[0], 0, 'f', 1));

    _aircraftHeading->setText(QString("%1°")
                                  .arg(track.getHeading(), 0, 'f', 1));

    _aircraftGroundTrack->setText(QString("%1°")
                                      .arg(track.getGroundTrack(), 0, 'f', 1));
}

void SectorDetailsPanel::setSector(int sectorId)
{
    SectorSummary sector = _dataService
                               ->getSectorSummaryData()
                               .getSectorSummary(sectorId);

    _selectedSectorId = sectorId;
    _selectedSectorIdx = QPoint(sector.getRow(), sector.getCol());
    _selectedAircraftId = "";

    refresh();
}

void SectorDetailsPanel::refresh()
{
    updateSectorStatusWidget();
    updateAircraftListWidget();
    updateSelectedAircraftWidget();
}

void SectorDetailsPanel::selectAircraft(QListWidgetItem *item)
{
    if (!item)
        return;

    QString ICAO = item->data(Qt::UserRole).toString();
    _selectedAircraftId = ICAO;

    updateSelectedAircraftWidget();
}