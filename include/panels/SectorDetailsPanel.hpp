#pragma once

#include <QWidget>
#include <domain/data/SectorSummary.hpp>

class QPoint;
class QLabel;
class QTabWidget;
class QListWidget;
class QListWidgetItem;

class IFlightDataService;

class SectorDetailsPanel : public QWidget
{
    Q_OBJECT

public slots:
    void setSector(int sectorId);
    void selectAircraft(QListWidgetItem *item);
    void refresh();

public:
    explicit SectorDetailsPanel(IFlightDataService &dataService, QWidget *parent = nullptr);

private:
    QWidget *buildSectorStatusWidget();
    QWidget *buildAircraftListWidget();
    QWidget *buildSelectedAircraftWidget();
    
    void updateSectorStatusWidget();
    void updateAircraftListWidget();
    void updateSelectedAircraftWidget();
    
    QPixmap loadStatusIcon(const QString &iconPath);

    QWidget *_sectorStatusWidget;
    QWidget *_aircraftListWidget;
    QWidget *_selectedAircraftWidget;

    QString _selectedAircraftId = "";
    QLabel *_aircraftListHeader = nullptr;
    QLabel *_aircraftLatitude = nullptr;
    QLabel *_aircraftLongitude = nullptr;
    QLabel *_aircraftAltitude = nullptr;
    QLabel *_aircraftGroundSpeed = nullptr;
    QLabel *_aircraftVertRate = nullptr;
    QLabel *_aircraftHeading = nullptr;
    QLabel *_aircraftGroundTrack = nullptr;

    int _selectedSectorId = -1;
    QPoint _selectedSectorIdx = {-1, -1};

    QLabel *_riskState;
    QLabel *_weatherState;
    QLabel *_trafficState;

    QLabel *_riskIcon = nullptr;
    QLabel *_weatherIcon = nullptr;
    QLabel *_trafficIcon = nullptr;

    QListWidget *_aircraftEntries;
    QLabel *_selectedAircraftHeader;
    QLabel *_selectedAircraftInfo;

    IFlightDataService &_dataService;
};