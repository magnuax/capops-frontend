#pragma once
#include <QFrame>

#include "domain/data/RiskEvent.hpp"

class QWidget;
class QTabWidget;
class QButtonGroup;
class AlertButton;

class AlertPanel : public QFrame
{
    Q_OBJECT

public:
    explicit AlertPanel(QWidget *parent = nullptr);

    QWidget *addAlert(const RiskEvent &riskEvent);

private:
    QTabWidget *buildAlertPanel();
    QWidget *buildAlertsTab();
    AlertButton *createAlertButton(int sectorId, const QString &label, const QDateTime &timestamp);

    QVector<AlertButton *> _activeAlerts;
    QWidget *_alertsTab = nullptr;

signals:
    void alertAcknowledged(const int &sectorId);
};