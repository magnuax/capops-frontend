#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QIcon>

#include "services/interfaces/IFlightDataService.hpp"
#include "panels/SectorDetailsPanel.hpp"
#include "domain/SectorStates.hpp"

SectorDetailsPanel::SectorDetailsPanel(IFlightDataService &dataService, QWidget *parent)
    : QWidget(parent),
      _dataService(dataService)
{
    auto *mainLayout = new QVBoxLayout(this);

    auto *tabs = new QTabWidget(this);
    mainLayout->addWidget(tabs);

    auto *sectorDetails = new QWidget(this);
    auto *detailsLayout = new QVBoxLayout(sectorDetails);

    detailsLayout->addWidget(buildSectorStatusWidget());
    detailsLayout->addStretch();
    detailsLayout->addWidget(buildAircraftListWidget());
    detailsLayout->addStretch();
    detailsLayout->addWidget(buildSelectedAircraftWidget());
    detailsLayout->addStretch();

    tabs->addTab(sectorDetails, "Details");

    setSector(0, 0);
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

    _riskState = new QLabel("-", _sectorStatusWidget);
    _weatherState = new QLabel("-", _sectorStatusWidget);
    _trafficState = new QLabel("-", _sectorStatusWidget);

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

    auto *header = new QLabel("Aircrafts", _aircraftListWidget);
    _aircraftListWidget->layout()->addWidget(header);
    _aircraftListWidget->layout()->addWidget(_aircraftEntries);

    connect(_aircraftEntries, &QListWidget::itemClicked, this, &SectorDetailsPanel::selectAircraft);

    return _aircraftListWidget;
}

QWidget *SectorDetailsPanel::buildSelectedAircraftWidget()
{
    _selectedAircraftWidget = new QWidget(this);
    _selectedAircraftWidget->setObjectName("SelectedAircraftWidget");

    _selectedAircraftHeader = new QLabel("No aircraft selected", _selectedAircraftWidget);
    _selectedAircraftInfo = new QLabel("Lorem ipsum dolor sit amet", _selectedAircraftWidget);

    _selectedAircraftWidget->setLayout(new QVBoxLayout());
    _selectedAircraftWidget->layout()->addWidget(_selectedAircraftHeader);
    _selectedAircraftWidget->layout()->addWidget(_selectedAircraftInfo);

    return _selectedAircraftWidget;
}

void SectorDetailsPanel::updateSectorStatusWidget()
{
    RiskState risk = _dataService.getRisk(_selectedSectorId);
    WeatherState weather = _dataService.getWeather(_selectedSectorId);
    TrafficState traffic = _dataService.getTraffic(_selectedSectorId);

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
    std::vector<int> flights = _dataService.getFlightIds(_selectedSectorId);

    _aircraftEntries->clear();

    for (int ICAO : flights)
    {
        QString text = QString("ICAO %1").arg(ICAO);
        auto *item = new QListWidgetItem(text, _aircraftEntries);
        item->setData(Qt::UserRole, ICAO);
    }
}

void SectorDetailsPanel::updateSelectedAircraftWidget()
{
    QString headerText = "No aircraft selected";

    if (_selectedAircraftId >= 0)
    {
        headerText = QString("Selected aircraft: %1").arg(_selectedAircraftId);
    }

    _selectedAircraftHeader->setText(headerText);

    // also update `_selectedAircraftInfo` when backend is ready
}

void SectorDetailsPanel::setSector(int row, int col)
{
    // PLACEHOLDER ID MAPPING:
    _selectedSectorIdx = {row, col};
    _selectedSectorId = row * _dataService.getColCount() + col;
    _selectedAircraftId = -1;

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

    int ICAO = item->data(Qt::UserRole).toInt();
    _selectedAircraftId = ICAO;

    updateSelectedAircraftWidget();
}