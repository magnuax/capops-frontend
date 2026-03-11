#pragma once
#include <QFrame>

class QWidget;
class QTabWidget;
class QButtonGroup;
class AlertButton;

class AlertPanel : public QFrame
{
    Q_OBJECT

public:
    explicit AlertPanel(QWidget *parent = nullptr);

private:
    QTabWidget *buildAlertPanel();
    QWidget *buildAlertsTab();

    QWidget *createAlertButton(int sectorId);

    QVector<AlertButton *> _activeAlerts;
    QWidget *_alertsTab = nullptr;

signals:
    void alertAcknowledged(const QString &sectorId);
};