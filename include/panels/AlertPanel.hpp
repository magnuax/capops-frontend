#pragma once
#include <QFrame>

#include "domain/data/RiskEvent.hpp"
#include "domain/data/RiskEventData.hpp"

class QWidget;
class QTabWidget;
class QButtonGroup;
class AlertButton;

class IFlightDataService;

class AlertPanel : public QFrame
{
    Q_OBJECT

signals:
    void alertAcknowledged(const MergedRiskEvent &mergedEvent);

private slots:
    void onAcknowledgeSucceeded(const MergedRiskEvent &mergedEvent);

public:
    explicit AlertPanel(IFlightDataService *dataService, QWidget *parent = nullptr);

    void clearAlerts();
    void addMergedAlert(const MergedRiskEvent &mergedEvent);

    void setRiskEventData(const RiskEventData &riskEventData);

private:
    QTabWidget *buildAlertPanel();
    QWidget *buildAlertsTab();

    AlertButton *findButtonForSector(int sectorId);

    void wireAcknowledgeButton(AlertButton *button);
    void handleAlertAcknowledged(AlertButton *button);

    QVector<AlertButton *> _activeAlerts;
    QWidget *_alertsContainer = nullptr;

    IFlightDataService *_dataService = nullptr;
};