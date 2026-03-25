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
    void handleAlertAcknowledged(AlertButton *button);

public:
    explicit AlertPanel(IFlightDataService *_dataService, QWidget *parent = nullptr);

    void clearAlerts();
    void clearPlaceholderAlerts();
    void addAlert(const RiskEvent &riskEvent);
    void addMergedAlert(const MergedRiskEvent &mergedEvent);

    void setRiskEventData(const RiskEventData &riskEventData);

private:
    QTabWidget *buildAlertPanel();
    QWidget *buildAlertsTab();

    void wireAcknowledgeButton(AlertButton *button);

    QVector<AlertButton *> _activeAlerts;
    QVector<AlertButton *> _placeholderAlerts;
    QWidget *_alertsTab = nullptr;

    IFlightDataService *_dataService = nullptr;
};