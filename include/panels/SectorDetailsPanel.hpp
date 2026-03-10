#pragma once

#include <QWidget>

class QPoint;
class QLabel;
class QTabWidget;
class QListWidget;
class QListWidgetItem;

class IFlightDataService;

class SectorDetailsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SectorDetailsPanel(IFlightDataService &dataService, QWidget *parent = nullptr);

public slots:
    void setSector(int row, int col);

    void selectAircraft(QListWidgetItem *item);

private:
    QWidget *createSectorStatusWidget();
    QWidget *createAircraftListWidget();
    QWidget *createSelectedAircraftWidget();
    
    void refresh();
    void updateSectorStatusWidget();
    void updateAircraftListWidget();
    void updateSelectedAircraftWidget();
    
    QPixmap loadStatusIcon(const QString &iconPath);
    QWidget *createSectionCard(const QString &objectName);

    void applyStyling();

    QWidget *_sectorStatusWidget;
    QWidget *_aircraftListWidget;
    QWidget *_selectedAircraftWidget;

    int _selectedAircraftId = -1;
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